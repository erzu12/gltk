#pragma once

#include "animation.h"
#include "events.h"
#include "list_resolvers.h"
#include "messure.h"
#include "renderables/renderable.h"
#include "vec_math.h"

#include <cassert>
#include <chrono>
#include <memory>
#include <optional>
#include <typeindex>
#include <vector>

namespace gltk {

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
    Animatable<int> top;
    Animatable<int> right;
    Animatable<int> bottom;
    Animatable<int> left;
};

enum class SizingMode { Layout, Content };

struct Sizing {
    SizingMode horizontal;
    SizingMode vertical;
};

struct Positioning {
    MessureVec2 size = MessureVec2(0_px, 0_px);
    MessureVec2 offset = MessureVec2(0_px, 0_px);
    Animatable<Vec2> anchor = Anchors::Center;
    Animatable<Vec2> pivot = Pivot::Center;
    Sizing sizing = {SizingMode::Layout, SizingMode::Layout};
    Padding padding = {0, 0, 0, 0};
    ChildPlacement childPlacement = ChildPlacement::Free;
    ListDirection listDirection = ListDirection::Down;
    Overflow overflow = Overflow::Scroll;
};

struct Transform {
    Vec2 Position;
    Vec2 Size;
    BoundingBox clipRegion;
};

struct Layout {
    size_t id = -1;
    std::optional<std::unique_ptr<IRenderable>> renderable;
    Positioning positioning;
    Transform transform;
    std::vector<Layout *> children;
    std::optional<Layout *> parent = std::nullopt;
    std::unordered_map<std::type_index, std::vector<std::function<void(IMouseEvent &)>>> eventCallbacks;

    template <typename T>
        requires std::derived_from<T, IRenderable>
    T *getRenderable() {
        if (!renderable.has_value()) {
            throw std::runtime_error("Renderable is not set for this layout");
        }
        T *ret = dynamic_cast<T *>(renderable.value().get());
        if (ret == nullptr) {
            throw std::runtime_error("Renderable is not of the requested type");
        }
        return ret;
    }

    Style *getStyle() {
        if (!renderable.has_value()) {
            throw std::runtime_error("Renderable is not set for this layout");
        }
        return renderable.value()->getStyle();
    }
};

class Scene {
    std::vector<std::unique_ptr<Layout>> layouts;
    std::optional<Layout *> root = std::nullopt;
    std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;

  public:
    Layout *addRelativeLayout(std::unique_ptr<Layout> layout);

    Layout *addRelativeLayout(std::unique_ptr<Layout> layout, Layout *parent);

    Layout *getRoot() const;

    void render() const;

    std::vector<Layout *> getLayouts() const;

    Layout *getLayout(size_t id) const { return layouts.at(id).get(); }

    template <typename T>
        requires std::derived_from<T, IMouseEvent>
    void addEventCallback(Layout *layout, std::function<void(T &)> callback) {
        auto wrapper = [callback](IMouseEvent &baseEvent) { callback(static_cast<T &>(baseEvent)); };
        layout->eventCallbacks[std::type_index(typeid(T))].push_back(wrapper);
    }

    template <typename T>
        requires std::derived_from<T, IMouseEvent>
    void sendEvent(T &event) {
        for (const auto &layout : layouts) {
            if (layout->transform.clipRegion.contains(event.getPos())) {
                for (const auto &callback : layout->eventCallbacks[std::type_index(typeid(T))]) {
                    event.localPos = event.getPos() - (layout->transform.Position - layout->transform.Size * 0.5f);
                    callback(event);
                }
            }
        }
    }

    void updateAnimations();
};

} // namespace gltk
