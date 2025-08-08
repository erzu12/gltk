#pragma once

#include "list_resolvers.h"
#include "messure.h"
#include "renderable.h"
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

enum class SizingMode { Layout, Content };

struct Sizing {
    SizingMode horizontal;
    SizingMode vertical;
};

struct Positioning {
    MessureVec2 size = MessureVec2(0_px, 0_px);
    MessureVec2 offset = MessureVec2(0_px, 0_px);
    Vec2 anchor = Anchors::Center;
    Vec2 pivot = Anchors::Center;
    Sizing sizing = {SizingMode::Layout, SizingMode::Layout};
    Padding padding = {0, 0, 0, 0};
    int test = 100;
    ChildPlacement childPlacement = ChildPlacement::Free;
    ListDirection listDirection = ListDirection::Down;
    Overflow overflow = Overflow::Scroll;
};

struct RelativeLayout {
    size_t id = -1;
    std::optional<std::unique_ptr<IRenderable>> renderable;
    Positioning positioning;
    Vec2 scrolePosition = Vec2(1, 1);
    std::vector<RelativeLayout *> children;
    std::optional<RelativeLayout *> parent = std::nullopt;
};

class RelativeScene {
    std::vector<std::unique_ptr<RelativeLayout>> layouts;
    std::optional<RelativeLayout *> root = std::nullopt;

  public:
    RelativeLayout *addRelativeLayout(std::unique_ptr<RelativeLayout> layout);

    RelativeLayout *addRelativeLayout(std::unique_ptr<RelativeLayout> layout, RelativeLayout *parent);

    RelativeLayout *getRoot() const;

    std::vector<RelativeLayout *> getLayouts() const;
};

} // namespace gltk
