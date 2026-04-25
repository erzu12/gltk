#pragma once

#include <animation.h>
#include <events.h>
#include <messure.h>
#include <renderables/renderable.h>
#include <vec_math.h>

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

enum class ListDirection {
    Down,
    Right,
    Left,
    Up,
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

/**
 * SizingMode determines how the size of a layout is calculated. Layout means the size is determined by the
 * layout system, while Content means the size is determined by the renderable content.
 * Setting one of the dimensions to Content and the other to Layout allows for control over the size while allowing the
 * content to keep its aspect ratio.
 */
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
    bool clipOverflow = true;
    bool visible = true;
    int zOffset = 0;
};

struct Transform {
    Vec2 Position;
    Vec2 Size;
    int zIndex;
    bool visible;
    BoundingBox clipBox;
    BoundingBox bbox;
};

class Layout {
    size_t id = -1;
    std::optional<std::unique_ptr<IRenderable>> renderable;
    Positioning positioning;
    Transform transform;
    std::vector<Layout *> children;
    std::optional<Layout *> parent = std::nullopt;
    std::unordered_map<std::type_index, std::vector<std::function<void(IEvent &)>>> eventCallbacks;
    bool isHovered = false;

    void sendHoverEvent(Vec2 pos, HoverState state) {
        MouseHoverEvent hoverEvent(pos);
        hoverEvent.state = state;
        for (const auto &callback : eventCallbacks[std::type_index(typeid(MouseHoverEvent))]) {
            callback(hoverEvent);
        }
    }

  public:
    Layout(Positioning positioning, std::optional<std::unique_ptr<IRenderable>> renderable = std::nullopt)
        : positioning(std::move(positioning)), renderable(std::move(renderable)) {}

    Positioning &getPositioning() { return positioning; }
    const Positioning &getPositioning() const { return positioning; }

    const Transform &getTransform() const { return transform; }

    std::vector<Layout *> &getChildren() { return children; }
    const std::vector<Layout *> &getChildren() const { return children; }

    std::optional<Layout *> getParent() const { return parent; }

    template <typename T>
        requires std::derived_from<T, IEvent>
    void sendEvent(T &event) {
        if (!transform.visible) {
            return;
        }
        if (eventCallbacks.find(std::type_index(typeid(MouseHoverEvent))) != eventCallbacks.end()) {
            if constexpr (std::is_same_v<T, MouseMoveEvent>) {
                if (transform.clipBox.contains(event.getPos()) && !isHovered) {
                    sendHoverEvent(event.getPos(), HoverState::ENTER);
                    isHovered = true;
                } else if (!transform.clipBox.contains(event.getPos()) && isHovered) {
                    sendHoverEvent(event.getPos(), HoverState::LEAVE);
                    isHovered = false;
                }
            }
        }
        if (eventCallbacks.find(std::type_index(typeid(T))) == eventCallbacks.end()) {
            return;
        }
        if (transform.clipBox.contains(event.getPos())) {
            for (const auto &callback : eventCallbacks[std::type_index(typeid(T))]) {
                if constexpr (std::derived_from<T, IMouseEvent>) {
                    event.localPos = event.getPos() - (transform.Position - transform.Size * 0.5f);
                }
                callback(event);
            }
        }
    }

    std::optional<IRenderable *> getRenderable() {
        if (!renderable.has_value()) {
            return std::nullopt;
        }
        return renderable.value().get();
    }

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

    template <typename T>
        requires std::derived_from<T, IEvent>
    void addEventCallback(std::function<void(T &)> callback) {
        auto wrapper = [callback](IEvent &baseEvent) { callback(static_cast<T &>(baseEvent)); };
        this->eventCallbacks[std::type_index(typeid(T))].push_back(wrapper);
    }

    Style *getStyle() {
        if (!renderable.has_value()) {
            throw std::runtime_error("Renderable is not set for this layout");
        }
        return renderable.value()->getStyle();
    }

    friend class Scene;
    friend class LayoutResolver;
};

class Scene {
    std::vector<std::unique_ptr<Layout>> layouts;
    std::optional<Layout *> root = std::nullopt;
    std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;

  public:
    Layout *addRelativeLayout(std::unique_ptr<Layout> layout);

    Layout *addRelativeLayout(std::unique_ptr<Layout> layout, Layout *parent);

    Layout *getRoot() const;

    void render();

    std::vector<Layout *> getLayouts() const;

    Layout *getLayout(const Layout *layout) const { return layouts.at(layout->id).get(); }

    template <typename T>
        requires std::derived_from<T, IMouseEvent>
    void sendEvent(T &event) {
        for (const auto &layout : layouts) {
            layout->sendEvent(event);
        }
    }
    void updateAnimations();
};

} // namespace gltk
