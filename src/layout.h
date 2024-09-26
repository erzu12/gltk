#pragma once

#include "vec_math.h"
#include "render.h"
#include "messure.h"
#include "list_resolvers.h"
#include <functional>
#include <memory>
#include <optional>
#include <vector>
#include <bitset>
#include <iostream>

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

enum class MouseButton {
    MOUSE_BUTTON_LEFT   = 0,
    MOUSE_BUTTON_RIGHT  = 1,
    MOUSE_BUTTON_MIDDLE = 2,
    MOUSE_BUTTON_4      = 3,
    MOUSE_BUTTON_5      = 4,
    MOUSE_BUTTON_6      = 5,
    MOUSE_BUTTON_7      = 6,
    MOUSE_BUTTON_8      = 7
};

enum class KeyModifiers {
    MOD_SHIFT   = 0x0001,
    MOD_CONTROL = 0x0002,
    MOD_ALT     = 0x0004,
    MOD_SUPER   = 0x0008,
    CAPS_LOCK   = 0x0010,
    NUM_LOCK    = 0x0020
};

class KeyModifierFlags {
    private:
    public:
        std::bitset<8> flags;
        KeyModifierFlags(int mods) : flags(mods) {}
        bool isModSet(KeyModifiers modifier) const {
            int bit = log2(static_cast<int>(modifier));
            return flags.test(bit);
        }
};

struct Positioning {
    MessureVec2 size = MessureVec2(0, 0);
    MessureVec2 offset = MessureVec2(0, 0);
    Vec2 anchor = Anchors::TopLeft;
    Vec2 pivot = Vec2(-1.0f);
    ChildPlacement childPlacement = ChildPlacement::Free;
    ListDirection listDirection = ListDirection::Down;
    Sizing verticalSizing = Sizing::Fixed;
    Sizing horizontalSizing = Sizing::Fixed;
    Overflow overflow = Overflow::Scroll;
};


class Layout {
public:
    Layout(MessureVec2 viewportSize, Renderer *renderer); // root layout defined by the window
    Layout(Layout *parent,
            Positioning positioning,
            std::unique_ptr<IRenderable> renderable = nullptr
    );

    void addChild(Layout *child);

    void setSize(MessureVec2 size);
    void scroll(Vec2 delta);

    void resolveTransform();

    void registerForRenderRecursive(BoundingBox clipRegion = BoundingBox(Vec2(0, 0), Vec2(1000000, 1000000)));

    void addOnMouseKeyDownCallback(std::function<void(MouseButton, KeyModifierFlags)> callback);
    void mouseKeyDownEventRecursive(Vec2 clickPosition, MouseButton button, KeyModifierFlags mods);

    void addOnMouseKeyUpCallback(std::function<void(MouseButton, KeyModifierFlags)> callback);
    void mouseKeyUpEventRecursive(Vec2 clickPosition, MouseButton button, KeyModifierFlags mods);

    void addOnScroleCallback(std::function<void(Vec2)> callback);
    bool scrollEventRecursive(Vec2 mousePosition, Vec2 scroleDelta);

    void setRenderable(std::unique_ptr<IRenderable> renderable);

    Mat3 getTransform();
    Vec2 getSize();

private:
    std::optional<std::unique_ptr<IRenderable>> renderable;
    std::vector<Layout*> children;
    std::optional<Layout*> parent;

    Renderer *renderer;

    Positioning positioning;

    Vec2 scrolePosition = Vec2(0, 0);

    std::optional<Vec2> resolvedPosition;
    std::optional<Vec2> resolvedSize;
    std::optional<Mat3> resolvedTransform;

    BoundingBox bounds;

    BoundingBox resolveTransform(Vec2 parentSize, Vec2 parentPosition, bool forceSize = false, ListDirection parentListDirection = ListDirection::Down);
    BoundingBox resolveChildTransforms(Vec2 parentSize, Vec2 parentPosition, ListDirection parentListDirection);
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

    std::vector<std::function<void(MouseButton, KeyModifierFlags)>> onMouseKeyDownCallbacks;
    std::vector<std::function<void(MouseButton, KeyModifierFlags)>> onMouseKeyUpCallbacks;
    std::vector<std::function<void(Vec2)>> onScrollCallbacks;
};

}
