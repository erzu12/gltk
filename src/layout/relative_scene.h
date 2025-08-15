#pragma once

#include "animation.h"
#include "events.h"
#include "list_resolvers.h"
#include "messure.h"
#include "renderable.h"
#include "vec_math.h"

#include <cassert>
#include <chrono>
#include <memory>
#include <optional>
#include <typeindex>
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

namespace Pivot = Anchors;

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
    Vec2 pivot = Pivot::Center;
    Sizing sizing = {SizingMode::Layout, SizingMode::Layout};
    Padding padding = {0, 0, 0, 0};
    ChildPlacement childPlacement = ChildPlacement::Free;
    ListDirection listDirection = ListDirection::Down;
    Overflow overflow = Overflow::Scroll;
    Vec2 scrolePosition = Vec2(1, 1);
};

struct RelativeLayout {
    size_t id = -1;
    std::optional<std::unique_ptr<IRenderable>> renderable;
    Positioning positioning;
    std::vector<RelativeLayout *> children;
    std::optional<RelativeLayout *> parent = std::nullopt;
    std::unordered_map<std::type_index, std::vector<std::function<void(IMouseEvent &)>>> eventCallbacks;
    std::vector<std::unique_ptr<IAnimationRunner>> animationRunners;

    template <typename T, typename V>
    void
    animate(T Positioning::*property, V endValue, float duration, std::function<float(float)> easingFunc = nullptr) {
        auto &pos = this->positioning;
        auto animationRunner = std::make_unique<AnimationRunner<V>>(&(pos.*property), endValue, duration, easingFunc);
        animationRunners.push_back(std::move(animationRunner));
    }

    template <typename T, typename V>
    void animate(T Padding::*property, V endValue, float duration, std::function<float(float)> easingFunc = nullptr) {
        auto &pos = this->positioning.padding;
        auto animationRunner = std::make_unique<AnimationRunner<V>>(&(pos.*property), endValue, duration, easingFunc);
        animationRunners.push_back(std::move(animationRunner));
    }
    template <typename T, typename V>
    void animate(T Style::*property, V endValue, float duration, std::function<float(float)> easingFunc = nullptr) {
        if (!renderable.has_value()) {
            throw std::runtime_error("Renderable is not set for this layout");
        }
        auto style = renderable.value()->getStyle();
        auto animationRunner =
            std::make_unique<AnimationRunner<V>>(&(style->*property), endValue, duration, easingFunc);
        animationRunners.push_back(std::move(animationRunner));
    }
};

class RelativeScene {
    std::vector<std::unique_ptr<RelativeLayout>> layouts;
    std::optional<RelativeLayout *> root = std::nullopt;
    std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;

  public:
    RelativeLayout *addRelativeLayout(std::unique_ptr<RelativeLayout> layout);

    RelativeLayout *addRelativeLayout(std::unique_ptr<RelativeLayout> layout, RelativeLayout *parent);

    RelativeLayout *getRoot() const;

    std::vector<RelativeLayout *> getLayouts() const;

    template <typename T>
        requires std::derived_from<T, IMouseEvent>
    void addEventCallback(std::function<void(T &)> callback, RelativeLayout *layout) {
        auto wrapper = [callback](IMouseEvent &baseEvent) { callback(static_cast<T &>(baseEvent)); };
        layout->eventCallbacks[std::type_index(typeid(T))].push_back(wrapper);
    }

    void updateAnimations();
};

} // namespace gltk
