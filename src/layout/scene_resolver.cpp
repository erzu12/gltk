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

void calculateTransform(RelativeLayout *layout, Vec2 parentSize, Vec2 parentPosition, ResolvedLayout *resolvedLayout) {
    Vec2 pivotPosition = layout->positioning.offset.resolve(parentSize) + layout->positioning.anchor * parentSize +
                         (parentPosition - parentSize / 2.0f);
    Vec2 size = layout->positioning.size.resolve(parentSize);
    if (layout->renderable.has_value()) {
        size = getFinalSize(size, layout->positioning.sizing, layout->renderable->get());
    }
    Vec2 pivotOffsetFromCenter = size / 2.0f - layout->positioning.pivot * size;
    resolvedLayout->Position = pivotPosition + pivotOffsetFromCenter;
    resolvedLayout->Size = size;
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
    for (auto &child : layout->children) {
        calculateTransform(child, paddedParentSize, paddedParentPosition, resolvedLayouts[child->id].get());
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

    calculateTransform(
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
