#include "scene_resolver.h"

namespace gltk {

Vec2 getFinalSize(Vec2 size, Sizing sizing, IRenderable *renderable) {
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

void calculateSize(RelativeLayout *layout, Vec2 parentSize, ResolvedLayout *resolvedLayout) {
    Vec2 size = layout->positioning.size.resolve(parentSize);
    if (layout->renderable.has_value()) {
        size = getFinalSize(size, layout->positioning.sizing, layout->renderable->get());
    }
    resolvedLayout->Size = size;
}

void calculatePosition(RelativeLayout *layout, Vec2 parentSize, Vec2 parentPosition, ResolvedLayout *resolvedLayout) {

    Vec2 pivotPosition = layout->positioning.offset.resolve(parentSize) + layout->positioning.anchor * parentSize +
                         (parentPosition - parentSize / 2.0f);
    Vec2 size = resolvedLayout->Size;
    Vec2 pivotOffsetFromCenter = size / 2.0f - layout->positioning.pivot * size;
    resolvedLayout->Position = pivotPosition + pivotOffsetFromCenter;
}

void calculateFreeTransforms(
    const std::vector<RelativeLayout *> &children,
    std::vector<std::unique_ptr<ResolvedLayout>> &resolvedLayouts,
    Vec2 parentSize,
    Vec2 parentPosition
) {
    for (const auto &child : children) {
        calculateSize(child, parentSize, resolvedLayouts[child->id].get());
        calculatePosition(child, parentSize, parentPosition, resolvedLayouts[child->id].get());
    }
}

void calculateListTransforms(
    const std::vector<RelativeLayout *> &children,
    std::vector<std::unique_ptr<ResolvedLayout>> &resolvedLayouts,
    Vec2 parentSize,
    Vec2 parentPosition,
    ListDirection listDirection
) {
    Vec2 dirVec = ListDireectionVector[static_cast<int>(listDirection)];
    int listAxis = ListDirectionAxis[static_cast<int>(listDirection)];
    Vec2 currentPosition = parentPosition - (dirVec * parentSize / 2.0f);
    for (const auto &child : children) {
        calculateSize(child, parentSize, resolvedLayouts[child->id].get());
        Vec2 childSize = resolvedLayouts[child->id]->Size;
        Vec2 listElementSize = childSize;
        listElementSize[listAxis ^ 1] = parentSize[listAxis ^ 1];
        calculatePosition(
            child, listElementSize, currentPosition + childSize * dirVec / 2.f, resolvedLayouts[child->id].get()
        );
        currentPosition += dirVec * childSize;
    }
}

void calculateListStretchTransforms(
    const std::vector<RelativeLayout *> &children,
    std::vector<std::unique_ptr<ResolvedLayout>> &resolvedLayouts,
    Vec2 parentSize,
    Vec2 parentPosition,
    ListDirection listDirection
) {
    Vec2 dirVec = ListDireectionVector[static_cast<int>(listDirection)];
    int listAxis = ListDirectionAxis[static_cast<int>(listDirection)];
    int totalAbsoluteSize = 0;
    float totalRelativeSize = 0;
    for (const auto &child : children) {
        calculateSize(child, parentSize, resolvedLayouts[child->id].get());
        Vec2 childSize = resolvedLayouts[child->id]->Size;
        if (child->positioning.size[listAxis]->isAbsolute()) {
            totalAbsoluteSize += childSize[listAxis];
        } else {
            totalRelativeSize += childSize[listAxis];
        }
    }

    Vec2 currentPosition = parentPosition - (dirVec * parentSize / 2.0f);
    float remainingSize = std::max(parentSize[listAxis] - totalAbsoluteSize, 0.f);
    for (const auto &child : children) {
        calculateSize(child, parentSize, resolvedLayouts[child->id].get());
        if (!child->positioning.size[listAxis]->isAbsolute()) {
            resolvedLayouts[child->id]->Size[listAxis] =
                remainingSize * (resolvedLayouts[child->id]->Size[listAxis] / totalRelativeSize);
        }
        Vec2 childSize = resolvedLayouts[child->id]->Size;
        Vec2 listElementSize = childSize;
        listElementSize[listAxis ^ 1] = parentSize[listAxis ^ 1];
        calculatePosition(
            child, listElementSize, currentPosition + childSize * dirVec / 2.f, resolvedLayouts[child->id].get()
        );
        currentPosition += dirVec * childSize;
    }
}

void resolveLayouts(
    const std::vector<RelativeLayout *> &relativeLayouts,
    const RelativeLayout *layout,
    std::vector<std::unique_ptr<ResolvedLayout>> &resolvedLayouts,
    const Vec2 viewportSize
) {
    const ResolvedLayout *resolvedLayout = resolvedLayouts[layout->id].get();
    Vec2 paddingTopLeftOffset(layout->positioning.padding.left, layout->positioning.padding.top);
    Vec2 paddingBottomRightOffset(layout->positioning.padding.right, layout->positioning.padding.bottom);
    Vec2 paddedParentSize = resolvedLayout->Size - paddingTopLeftOffset - paddingBottomRightOffset;
    Vec2 paddedParentPosition = resolvedLayout->Position + (paddingTopLeftOffset - paddingBottomRightOffset) / 2.0f;
    switch (layout->positioning.childPlacement) {
    case ChildPlacement::Free:
        calculateFreeTransforms(layout->children, resolvedLayouts, paddedParentSize, paddedParentPosition);
        break;
    case ChildPlacement::List:
        calculateListTransforms(
            layout->children, resolvedLayouts, paddedParentSize, paddedParentPosition, layout->positioning.listDirection
        );
        break;
    case ChildPlacement::ListStretch:
        calculateListStretchTransforms(
            layout->children, resolvedLayouts, paddedParentSize, paddedParentPosition, layout->positioning.listDirection
        );
        break;
    }
    for (auto &child : layout->children) {
        resolveLayouts(relativeLayouts, child, resolvedLayouts, viewportSize);
    }
}

std::unique_ptr<ResolvedScene> resolveScene(const RelativeScene &scene, Vec2 viewportSize) {
    auto relativeLayouts = scene.getLayouts();
    std::vector<std::unique_ptr<ResolvedLayout>> resolvedLayouts(relativeLayouts.size());
    for (size_t i = 0; i < relativeLayouts.size(); ++i) {
        resolvedLayouts[i] = std::make_unique<ResolvedLayout>();
        if (relativeLayouts[i]->renderable.has_value()) {
            resolvedLayouts[i]->renderable = relativeLayouts[i]->renderable->get();
        }
    }

    calculateSize(relativeLayouts[scene.getRoot()->id], viewportSize, resolvedLayouts[scene.getRoot()->id].get());
    calculatePosition(
        relativeLayouts[scene.getRoot()->id],
        viewportSize,
        viewportSize / 2.0f,
        resolvedLayouts[scene.getRoot()->id].get()
    );

    resolveLayouts(relativeLayouts, scene.getRoot(), resolvedLayouts, viewportSize);

    for (auto &relativeLayout : relativeLayouts) {
        const auto &resolvedLayout = resolvedLayouts[relativeLayout->id];
        for (auto &child : relativeLayout->children) {
            resolvedLayout->children.push_back(resolvedLayouts[child->id].get());
        }
    }

    auto resolvedScene = std::make_unique<ResolvedScene>(std::move(resolvedLayouts), scene.getRoot()->id);

    return resolvedScene;
}

} // namespace gltk
