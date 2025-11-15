#include "scene.h"

namespace gltk {

Layout *Scene::addRelativeLayout(std::unique_ptr<Layout> layout) {
    if (root.has_value()) {
        Layout *old_root = root.value();
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

Layout *Scene::addRelativeLayout(std::unique_ptr<Layout> layout, Layout *parent) {
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

Layout *Scene::getRoot() const {
    if (!root.has_value()) {
        throw std::runtime_error("Root layout is not set");
    }
    return root.value();
}

std::vector<Layout *> Scene::getLayouts() const {
    std::vector<Layout *> result;
    for (auto &layout : layouts) {
        result.push_back(layout.get());
    }
    return result;
}

void Scene::updateAnimations() {
    auto now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(now - lastUpdateTime).count();
    lastUpdateTime = now;
    for (auto &layout : layouts) {
        for (auto &runner : layout->animationRunners) {
            runner->update(deltaTime);
        }
    }
}

void Scene::render() const {
    if (!root.has_value()) {
        return;
    }
    for (const auto &layout : layouts) {
        if (layout->renderable.has_value()) {
            Mat3 modelMatrix = Mat3::translationMatrix(layout->transform.Position);
            modelMatrix = modelMatrix * Mat3::scalingMatrix(layout->transform.Size);
            layout->renderable.value()->render(
                root.value()->transform.Size, modelMatrix, layout->transform.Size, layout->transform.clipRegion
            );
        }
    }
}

}; // namespace gltk
