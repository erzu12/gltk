#pragma once

#include "vec_math.h"
#include "render.h"
#include "messure.h"
#include "list_resolvers.h"
#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace gltk {

class Layout;

namespace Anchors {
    const Vec2 TopLeft = Vec2(0, 0);
    const Vec2 TopCenter = Vec2(0.5, 0);
    const Vec2 TopRight = Vec2(1, 0);
    const Vec2 CenterLeft = Vec2(0, 0.5);
    const Vec2 Center = Vec2(0.5, 0.5);
    const Vec2 CenterRight = Vec2(1, 0.5);
    const Vec2 BottomLeft = Vec2(0, 1);
    const Vec2 BottomCenter = Vec2(0.5, 1);
    const Vec2 BottomRight = Vec2(1, 1);
}


enum class ChildPlacement {
    Free,
    List,
    ListStretch,
};


enum class Overflow {
    None,
    Clip,
    Scroll,
};


class Layout {
public:
    Layout(MessureVec2 viewportSize); // root layout defined by the window
    Layout(Layout *parent,
           Vec2 anchor,
           MessureVec2 offset,
           Vec2 pivot,
           MessureVec2 size,
           ChildPlacement childPlacement = ChildPlacement::Free,
           ListDirection listDirection = ListDirection::Down,
           Sizing horizontalSizing = Sizing::Fixed,
           Sizing verticalSizing = Sizing::Fixed,
           std::unique_ptr<IRenderable> renderable = nullptr,
           Overflow overflow = Overflow::None
    );

    void addChild(Layout *child);

    void setSize(MessureVec2 size);
    void scroll(Vec2 delta);

    void resolveTransform();

    void registerForRenderRecursive(Renderer &renderer);

    void addOnClickCallback(std::function<void()> callback);
    void clickEventRecursive(Vec2 clickPosition);

    void addOnScroleCallback(std::function<void(Vec2)> callback);
    bool scrollEventRecursive(Vec2 mousePosition, Vec2 scroleDelta);

    Mat3 getTransform();
    Vec2 getSize();

private:
    std::optional<std::unique_ptr<IRenderable>> renderable;
    std::vector<Layout*> children;
    std::optional<Layout*> parent;

    Vec2 anchor = Anchors::TopLeft;
    MessureVec2 offset;
    Vec2 pivot = Anchors::TopLeft;
    MessureVec2 size;

    ChildPlacement childPlacement;
    ListDirection listDirection;
    Sizing verticalSizing;
    Sizing horizontalSizing;
    Overflow overflow;

    Vec2 scrolePosition = Vec2(0, 0);

    std::optional<Vec2> resolvedPosition;
    std::optional<Vec2> resolvedSize;
    std::optional<Mat3> resolvedTransform;

    BoundingBox bounds;

    BoundingBox resolveTransform(Vec2 parentSize, Vec2 parentPosition, bool forceSize = false, ListDirection parentListDirection = ListDirection::Down);
    void recalculateTransformFromBounds(BoundingBox bounds);
    void calculateTransform(Vec2 parentSize, Vec2 parentPosition, bool forceSize, ListDirection parentListDirection);
    void moveChildren(Vec2 delta);
    void boundScrolePosition(BoundingBox childBounds);

    BoundingBox resolveListTransform();
    BoundingBox resolveListStretchTransform();

    BoundingBox getRenderableBounds(Vec2 parentSize, Vec2 parentPosition);

    void adjustCurrentPosition(Vec2 childSize, Vec2 &currentPosition);
    Vec2 getListStartPossition();
    Vec2 getListParentSize(Vec2 childSize);

    std::vector<std::function<void()>> onClickCallbacks;
    std::vector<std::function<void(Vec2)>> onScrollCallbacks;
};

}
