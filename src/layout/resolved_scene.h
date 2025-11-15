#pragma once

#include "events.h"
#include "renderables/renderable.h"
#include <functional>
#include <memory>
#include <optional>
#include <typeindex>

namespace gltk {

struct ResolvedLayout {
    std::optional<IRenderable *> renderable;
    Vec2 Position;
    Vec2 Size;
    BoundingBox clipRegion;
    std::vector<ResolvedLayout *> children;
    std::unordered_map<std::type_index, std::vector<std::function<void(IMouseEvent &)>>> eventCallbacks;
};

class ResolvedScene {
    std::optional<ResolvedLayout *> root;
    std::vector<std::unique_ptr<ResolvedLayout>> layouts;

  public:
    explicit ResolvedScene(std::vector<std::unique_ptr<ResolvedLayout>> layouts, int rootId)
        : layouts(std::move(layouts)) {
        if (this->layouts.empty()) {
            throw std::runtime_error("ResolvedScene must contain at least one layout");
        }
        root = this->layouts[rootId].get();
    }

    void render() const;

    template <typename T>
        requires std::derived_from<T, IMouseEvent>
    void sendEvent(T &event) {
        for (const auto &layout : layouts) {
            if (layout->clipRegion.contains(event.getPos())) {
                for (const auto &callback : layout->eventCallbacks[std::type_index(typeid(T))]) {
                    callback(event);
                }
            }
        }
    }

    ResolvedLayout *getRoot() const {
        if (!root.has_value()) {
            throw std::runtime_error("Root layout is not set");
        }
        return root.value();
    }

    std::vector<ResolvedLayout *> getLayouts() const {
        std::vector<ResolvedLayout *> result;
        for (const auto &layout : layouts) {
            result.push_back(layout.get());
        }
        return result;
    }

    ResolvedLayout *getLayout(size_t id) const { return layouts[id].get(); }
};

} // namespace gltk
