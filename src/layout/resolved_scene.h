#pragma once

#include "render.h"
#include "tree.h"

namespace gltk {

struct ResolvedLayout {
    std::optional<std::unique_ptr<IRenderable>> renderable;
    Renderer *renderer;
    Vec2 Position;
    Vec2 Size;
    Mat3 Transform;
    std::vector<ResolvedLayout *> children;
};

class ResolvedScene {
    std::unique_ptr<ResolvedLayout> root;
    std::vector<std::unique_ptr<ResolvedLayout>> layouts;
public:
    explicit ResolvedScene(std::vector<std::unique_ptr<ResolvedLayout>> layouts) : layouts(std::move(layouts)) {
        root = std::move(this->layouts[0]);
    }

    ResolvedLayout *getRoot() const {
        return root.get();
    }

    std::vector<ResolvedLayout *> getLayouts() const {
        std::vector<ResolvedLayout *> result;
        for (const auto &layout : layouts) {
            result.push_back(layout.get());
        }
        return result;
    }

    ResolvedLayout *getLayout(size_t id) const {
        return layouts[id].get();
    }
};
} // namespace gltk
