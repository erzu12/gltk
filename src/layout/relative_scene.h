#pragma once

#include "list_resolvers.h"
#include "messure.h"
#include "render.h"
#include "vec_math.h"

#include <cassert>
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
} // namespace Anchors

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

struct Padding {
    int top;
    int right;
    int bottom;
    int left;
};

struct Positioning {
    MessureVec2 size = MessureVec2(0_px, 0_px);
    MessureVec2 offset = MessureVec2(0_px, 0_px);
    Vec2 anchor = Anchors::Center;
    Vec2 pivot = Anchors::Center;
    Padding padding = {0, 0, 0, 0};
    int test = 100;
    ChildPlacement childPlacement = ChildPlacement::Free;
    ListDirection listDirection = ListDirection::Down;
    Sizing verticalSizing = Sizing::Fixed;
    Sizing horizontalSizing = Sizing::Fixed;
    Overflow overflow = Overflow::Scroll;
};

struct RelativeLayout {
    size_t id = -1;
    std::optional<std::unique_ptr<IRenderable>> renderable;
    Positioning positioning;
    Vec2 scrolePosition = Vec2(1, 1);
    std::vector<RelativeLayout *> children;
    std::optional<RelativeLayout *>parent = std::nullopt;

    bool hasFixedHeight() {
        return positioning.size.y->isAbsolute() || (renderable.has_value() && positioning.verticalSizing == Sizing::Fit);
    }

    bool hasFixedWidth() {
        return positioning.size.x->isAbsolute() || (renderable.has_value() && positioning.horizontalSizing == Sizing::Fit);
    }
};

class RelativeScene {
    std::vector<std::unique_ptr<RelativeLayout>> layouts;
    std::optional<RelativeLayout *> root;

public:

    RelativeLayout *addRelativeLayout(std::unique_ptr<RelativeLayout> layout) {
        if (root.has_value()) {
            RelativeLayout *old_root = root.value();
            root = layout.get();
            root.value()->children.push_back(old_root);
        } else {
            root = layout.get();
        }
        if (layout->id == -1) {
            layout->id = layouts.size();
        } else {
            throw std::invalid_argument("Layout ID must be -1");
        }
        layouts.push_back(std::move(layout));
        return layouts.back().get();
    }

    RelativeLayout *addRelativeLayout(std::unique_ptr<RelativeLayout> layout, RelativeLayout *parent) {
        if (!parent) throw std::invalid_argument("Parent layout cannot be null");
        parent->children.push_back(layout.get());
        if (layout->id == -1) {
            layout->id = layouts.size();
        } else {
            throw std::invalid_argument("Layout ID must be -1");
        }
        layouts.push_back(std::move(layout));
        return layouts.back().get();
    }

    RelativeLayout *getRoot() const {
        return root.value();
    }

    std::vector<RelativeLayout *> getLayouts() const {
        std::vector<RelativeLayout *> result;
        for (auto &layout : layouts) {
            result.push_back(layout.get());
        }
        return result;
    }
};

} // namespace gltk
