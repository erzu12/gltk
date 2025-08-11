#include "relative_scene.h"

namespace gltk {

RelativeLayout *RelativeScene::addRelativeLayout(std::unique_ptr<RelativeLayout> layout) {
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

RelativeLayout *RelativeScene::addRelativeLayout(std::unique_ptr<RelativeLayout> layout, RelativeLayout *parent) {
    if (!parent)
        throw std::invalid_argument("Parent layout cannot be null");
    parent->children.push_back(layout.get());
    layout->parent = parent;
    if (layout->id == -1) {
        layout->id = layouts.size();
    } else {
        throw std::invalid_argument("Layout ID must be -1");
    }
    layouts.push_back(std::move(layout));
    return layouts.back().get();
}

RelativeLayout *RelativeScene::getRoot() const {
    if (!root.has_value()) {
        throw std::runtime_error("Root layout is not set");
    }
    return root.value();
}

std::vector<RelativeLayout *> RelativeScene::getLayouts() const {
    std::vector<RelativeLayout *> result;
    for (auto &layout : layouts) {
        result.push_back(layout.get());
    }
    return result;
}

}; // namespace gltk
