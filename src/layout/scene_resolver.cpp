#include "scene_resolver.h"

namespace gltk {

const Vec2 LayoutResolver::ListDireectionVector[4] = {
    Vec2(0, 1),  // Down
    Vec2(1, 0),  // Right
    Vec2(-1, 0), // Left
    Vec2(0, -1), // Up
};

const int LayoutResolver::ListDirectionAxis[4] = {
    1, // Down
    0, // Right
    0, // Left
    1, // Up
};

Vec2 LayoutResolver::getFinalSize(Vec2 size, Sizing sizing, IRenderable *renderable) {
    Vec2 finalSize = size;
    Vec2 renderableSize =
        renderable->getSize(size, sizing.horizontal == SizingMode::Layout, sizing.vertical == SizingMode::Layout);
    if (sizing.horizontal == SizingMode::Content) {
        finalSize.x = renderableSize.x;
    }
    if (sizing.vertical == SizingMode::Content) {
        finalSize.y = renderableSize.y;
    }
    return finalSize;
}

void LayoutResolver::calculateSize(Layout *layout, Vec2 parentSize) {
    Vec2 size = layout->positioning.size.resolve(parentSize);
    if (layout->renderable.has_value()) {
        size = getFinalSize(size, layout->positioning.sizing, layout->renderable->get());
    }
    layout->transform.Size = size;
}

void LayoutResolver::calculatePosition(Layout *layout, Vec2 parentSize, Vec2 parentPosition) {

    Vec2 pivotPosition = layout->positioning.offset.resolve(parentSize) +
                         layout->positioning.anchor.get() * parentSize + (parentPosition - parentSize / 2.0f);
    Vec2 size = layout->transform.Size;
    Vec2 pivotOffsetFromCenter = size / 2.0f - layout->positioning.pivot.get() * size;
    layout->transform.Position = pivotPosition + pivotOffsetFromCenter;
}

void LayoutResolver::calculateFreeTransforms(
    const std::vector<Layout *> &children, Vec2 parentSize, Vec2 parentPosition
) {
    for (const auto &child : children) {
        calculateSize(child, parentSize);
        calculatePosition(child, parentSize, parentPosition);
    }
}

void LayoutResolver::calculateListTransforms(
    const std::vector<Layout *> &children, Vec2 parentSize, Vec2 parentPosition, ListDirection listDirection
) {
    Vec2 dirVec = ListDireectionVector[static_cast<int>(listDirection)];
    int listAxis = ListDirectionAxis[static_cast<int>(listDirection)];
    Vec2 currentPosition = parentPosition - (dirVec * parentSize / 2.0f);
    for (const auto &child : children) {
        calculateSize(child, parentSize);
        Vec2 childSize = child->transform.Size;
        Vec2 listElementSize = childSize;
        listElementSize[listAxis ^ 1] = parentSize[listAxis ^ 1];
        calculatePosition(child, listElementSize, currentPosition + childSize * dirVec / 2.f);
        currentPosition += dirVec * childSize;
    }
}

void LayoutResolver::calculateListStretchTransforms(
    const std::vector<Layout *> &children, Vec2 parentSize, Vec2 parentPosition, ListDirection listDirection
) {
    Vec2 dirVec = ListDireectionVector[static_cast<int>(listDirection)];
    int listAxis = ListDirectionAxis[static_cast<int>(listDirection)];
    int totalAbsoluteSize = 0;
    float totalRelativeSize = 0;
    for (const auto &child : children) {
        calculateSize(child, parentSize);
        Vec2 childSize = child->transform.Size;
        if (child->positioning.size[listAxis]->isAbsolute()) {
            totalAbsoluteSize += childSize[listAxis];
        } else {
            totalRelativeSize += childSize[listAxis];
        }
    }

    Vec2 currentPosition = parentPosition - (dirVec * parentSize / 2.0f);
    float remainingSize = std::max(parentSize[listAxis] - totalAbsoluteSize, 0.f);
    for (const auto &child : children) {
        calculateSize(child, parentSize);
        if (!child->positioning.size[listAxis]->isAbsolute()) {
            child->transform.Size[listAxis] = remainingSize * (child->transform.Size[listAxis] / totalRelativeSize);
        }
        Vec2 childSize = child->transform.Size;
        Vec2 listElementSize = childSize;
        listElementSize[listAxis ^ 1] = parentSize[listAxis ^ 1];
        calculatePosition(child, listElementSize, currentPosition + childSize * dirVec / 2.f);
        currentPosition += dirVec * childSize;
    }
}

void LayoutResolver::resolveLayouts(
    const std::vector<Layout *> &relativeLayouts, const Layout *parent, const Vec2 viewportSize
) {
    Vec2 paddingTopLeftOffset(parent->positioning.padding.left, parent->positioning.padding.top);
    Vec2 paddingBottomRightOffset(parent->positioning.padding.right, parent->positioning.padding.bottom);
    Vec2 paddedParentSize = parent->transform.Size - paddingTopLeftOffset - paddingBottomRightOffset;
    Vec2 paddedParentPosition = parent->transform.Position + (paddingTopLeftOffset - paddingBottomRightOffset) / 2.0f;
    switch (parent->positioning.childPlacement) {
    case ChildPlacement::Free:
        calculateFreeTransforms(parent->children, paddedParentSize, paddedParentPosition);
        break;
    case ChildPlacement::List:
        calculateListTransforms(
            parent->children, paddedParentSize, paddedParentPosition, parent->positioning.listDirection
        );
        break;
    case ChildPlacement::ListStretch:
        calculateListStretchTransforms(
            parent->children, paddedParentSize, paddedParentPosition, parent->positioning.listDirection
        );
        break;
    }
    for (auto &child : parent->children) {
        BoundingBox boundingBox = BoundingBox(child->transform.Position, child->transform.Size);
        child->transform.bbox = boundingBox;
        if (child->positioning.clipOverflow) {
            child->transform.clipBox = boundingBox.intersect(parent->transform.clipBox);
        } else {
            child->transform.clipBox = boundingBox;
        }
        child->transform.zIndex = parent->transform.zIndex + child->positioning.zOffset;
        child->transform.visible = parent->transform.visible && child->positioning.visible;
        resolveLayouts(relativeLayouts, child, viewportSize);
    }
}

void LayoutResolver::resolveRootLayout(Layout *rootLayout, Vec2 viewportSize) {
    calculateSize(rootLayout, viewportSize);
    calculatePosition(rootLayout, viewportSize, viewportSize / 2.0f);
    rootLayout->transform.clipBox = BoundingBox(rootLayout->transform.Position, rootLayout->transform.Size);
    rootLayout->transform.bbox = rootLayout->transform.clipBox;
    rootLayout->transform.zIndex = 0;
    rootLayout->transform.visible = true;
}

void LayoutResolver::sendTransformChangeEvents(
    const std::vector<Layout *> &relativeLayouts, const std::vector<Transform> &startTransfomrs
) {
    for (size_t i = 0; i < relativeLayouts.size(); i++) {
        if (relativeLayouts[i]->transform.Position != startTransfomrs[i].Position ||
            relativeLayouts[i]->transform.Size != startTransfomrs[i].Size) {
            for (const auto &callback :
                 relativeLayouts[i]->eventCallbacks[std::type_index(typeid(TransformChangeEvent))]) {
                TransformChangeEvent event;
                event.pos = relativeLayouts[i]->transform.Position;
                event.size = relativeLayouts[i]->transform.Size;
                callback(event);
            }
        }
    }
}

void LayoutResolver::resolveScene(Scene &scene, Vec2 viewportSize) {
    scene.updateAnimations();

    auto relativeLayouts = scene.getLayouts();
    auto startTransfomrs = std::vector<Transform>(relativeLayouts.size());
    for (size_t i = 0; i < relativeLayouts.size(); i++) {
        startTransfomrs[i] = relativeLayouts[i]->transform;
    }

    resolveRootLayout(scene.getRoot(), viewportSize);

    resolveLayouts(relativeLayouts, scene.getRoot(), viewportSize);

    sendTransformChangeEvents(relativeLayouts, startTransfomrs);
}

} // namespace gltk
