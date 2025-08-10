#pragma once

#include "renderable.h"
#include <memory>
#include <optional>

namespace gltk {

struct ResolvedLayout {
    std::optional<IRenderable *> renderable;
    Vec2 Position;
    Vec2 Size;
    BoundingBox clipRegion;
    std::vector<ResolvedLayout *> children;
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
