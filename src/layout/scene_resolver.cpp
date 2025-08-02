#include "scene_resolver.h"
#include <algorithm>

namespace gltk {

struct ResolveData {
    float minWidth = 0;
    float minHeight = 0;
    float maxWidth = std::numeric_limits<float>::infinity();
    float maxHeight = std::numeric_limits<float>::infinity();
    Vec2 pivotPosition = Vec2(-1, -1);
    Vec2 centerPosition = Vec2(-1, -1);

    void setSize(Vec2 size) {
        minWidth = size.x;
        minHeight = size.y;
        maxWidth = size.x;
        maxHeight = size.y;
    }

    Vec2 getSize() const { return Vec2(minWidth, minHeight); }

    Mat3 getTransform() const {
        Mat3 transform = Mat3::translationMatrix(centerPosition);
        transform = transform * Mat3::scalingMatrix(Vec2(maxWidth, maxHeight));
        return transform;
    }

    bool isFullyResolved() const {
        return minWidth == maxWidth && minHeight == maxHeight && pivotPosition.x >= 0 && pivotPosition.y >= 0 &&
               centerPosition.x >= 0 && centerPosition.y >= 0;
    }
};

void resolveAbsoluteHeightParentLayouts(const RelativeLayout *layout, std::vector<ResolveData> &resolveData) {
    using enum Sizing;
    if (!layout->parent.has_value() || layout->parent.value()->children.size() != 1) {
        // throw std::invalid_argument("not implemented");
        return;
    }
    Sizing parentSizing = layout->parent.value()->positioning.verticalSizing;
    if (parentSizing == Fixed) {
        return;
    }

    if (resolveData[layout->id].minHeight > 0 && (parentSizing == Fit || parentSizing == Expand)) {
        size_t parentId = layout->parent.value()->id;
        resolveData[parentId].minHeight = resolveData[layout->id].minHeight;
    }
    if (resolveData[layout->id].maxHeight < std::numeric_limits<float>::infinity() &&
        (parentSizing == Fit || parentSizing == Expand)) {
        size_t parentId = layout->parent.value()->id;
        resolveData[parentId].maxHeight = resolveData[layout->id].maxHeight;
    }
    resolveAbsoluteHeightParentLayouts(layout->parent.value(), resolveData);
}

void resolveAbsoluteHeightLayouts(
    const std::vector<RelativeLayout *> &fixedLayouts, std::vector<ResolveData> &resolveData, Vec2 viewportSize
) {
    for (auto &layout : fixedLayouts) {
        using enum Sizing;
        if (layout->renderable.has_value()) {
            if (layout->positioning.verticalSizing == Fit || layout->positioning.verticalSizing == Shrink) {
                resolveData[layout->id].maxHeight = layout->renderable.value()->getSize(viewportSize, true, false).y;
            }
            if (layout->positioning.verticalSizing == Fit || layout->positioning.verticalSizing == Expand) {
                resolveData[layout->id].minHeight = layout->renderable.value()->getSize(viewportSize, true, false).y;
            }
        }
        if (layout->positioning.size.y->isAbsolute() && layout->positioning.verticalSizing == Fixed) {
            resolveData[layout->id].minHeight = layout->positioning.size.y->resolve(viewportSize.y);
            resolveData[layout->id].maxHeight = layout->positioning.size.y->resolve(viewportSize.y);
        }
        resolveAbsoluteHeightParentLayouts(layout, resolveData);
    }
}

void resolveAbsoluteWidthParentLayouts(const RelativeLayout *layout, std::vector<ResolveData> &resolveData) {
    using enum Sizing;
    if (!layout->parent.has_value() || layout->parent.value()->children.size() != 1) {
        // throw std::invalid_argument("not implemented");
        return;
    }
    Sizing parentSizing = layout->parent.value()->positioning.horizontalSizing;
    if (parentSizing == Fixed) {
        return;
    }

    if (resolveData[layout->id].minWidth > 0 && (parentSizing == Fit || parentSizing == Expand)) {
        size_t parentId = layout->parent.value()->id;
        resolveData[parentId].minWidth = resolveData[layout->id].minWidth;
    }
    if (resolveData[layout->id].maxWidth < std::numeric_limits<float>::infinity() &&
        (parentSizing == Fit || parentSizing == Expand)) {
        size_t parentId = layout->parent.value()->id;
        resolveData[parentId].maxWidth = resolveData[layout->id].maxWidth;
    }
    resolveAbsoluteWidthParentLayouts(layout->parent.value(), resolveData);
}

void resolveAbsoluteWidthLayouts(
    const std::vector<RelativeLayout *> &fixedLayouts, std::vector<ResolveData> &resolveData, Vec2 viewportSize
) {
    for (auto &layout : fixedLayouts) {
        using enum Sizing;
        if (layout->renderable.has_value()) {
            if (layout->positioning.horizontalSizing == Fit || layout->positioning.horizontalSizing == Shrink) {
                resolveData[layout->id].maxWidth = layout->renderable.value()->getSize(viewportSize, false, true).x;
            }
            if (layout->positioning.horizontalSizing == Fit || layout->positioning.horizontalSizing == Expand) {
                resolveData[layout->id].minWidth = layout->renderable.value()->getSize(viewportSize, false, true).x;
            }
        }
        if (layout->positioning.size.x->isAbsolute() && layout->positioning.horizontalSizing == Fixed) {
            resolveData[layout->id].minWidth = layout->positioning.size.x->resolve(viewportSize.x);
            resolveData[layout->id].maxWidth = layout->positioning.size.x->resolve(viewportSize.x);
        }
        resolveAbsoluteWidthParentLayouts(layout, resolveData);
    }
}

void calculateTransform(RelativeLayout *layout, Vec2 parentSize, Vec2 parentPosition, ResolveData *resolveData) {
    Vec2 pivotPosition =
        layout->positioning.offset.resolve(parentSize) + layout->positioning.anchor * parentSize + parentPosition;
    Vec2 size = layout->positioning.size.resolve(parentSize);
    size = Vec2(
        std::clamp(size.x, resolveData->minWidth, resolveData->maxWidth),
        std::clamp(size.y, resolveData->minHeight, resolveData->maxHeight)
    );
    Vec2 pivotOffsetFromCenter = size / 2.0f - layout->positioning.pivot * size;
    resolveData->centerPosition = pivotPosition + pivotOffsetFromCenter;
    resolveData->pivotPosition = pivotPosition - size * layout->positioning.pivot;
    resolveData->setSize(size);
}

void resolveRelativeLayouts(
    const std::vector<RelativeLayout *> &relativeLayouts,
    const RelativeLayout *layout,
    std::vector<ResolveData> &resolveData,
    const Vec2 viewportSize
) {
    const ResolveData *layoutResolveData = &resolveData[layout->id];
    if (!layoutResolveData->isFullyResolved()) {
        throw std::invalid_argument("the root layout must be fully resolved");
        return;
    }
    for (auto &child : layout->children) {
        calculateTransform(
            child, layoutResolveData->getSize(), layoutResolveData->pivotPosition, &resolveData[child->id]
        );
        resolveRelativeLayouts(relativeLayouts, child, resolveData, viewportSize);
    }
}

std::unique_ptr<ResolvedScene> resolveScene(const RelativeScene &scene, Vec2 viewportSize) {
    auto relativeLayouts = scene.getLayouts();
    std::vector<ResolveData> resolveData(relativeLayouts.size());
    resolveAbsoluteWidthLayouts(relativeLayouts, resolveData, viewportSize);
    resolveAbsoluteHeightLayouts(relativeLayouts, resolveData, viewportSize);

    calculateTransform(
        relativeLayouts[scene.getRoot()->id], viewportSize, Vec2(0, 0), &resolveData[scene.getRoot()->id]
    );

    resolveRelativeLayouts(relativeLayouts, scene.getRoot(), resolveData, viewportSize);

    std::vector<std::unique_ptr<ResolvedLayout>> resolvedLayouts;
    for (const auto &data : resolveData) {
        resolvedLayouts.emplace_back(std::make_unique<ResolvedLayout>());
        resolvedLayouts.back()->Position = data.centerPosition;
        resolvedLayouts.back()->Size = data.getSize();
        resolvedLayouts.back()->Transform = data.getTransform();
    }
    for (auto &relativeLayout : relativeLayouts) {
        const auto &resolvedLayout = resolvedLayouts[relativeLayout->id];
        for (auto &child : relativeLayout->children) {
            resolvedLayout->children.push_back(resolvedLayouts[child->id].get());
        }
    }

    auto resolvedScene = std::make_unique<ResolvedScene>(std::move(resolvedLayouts));

    return resolvedScene;
}

} // namespace gltk
