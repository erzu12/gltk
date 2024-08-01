#include "node.h"

#include <algorithm>

namespace gltk {


Node::Node(MessureVec2 viewportSize) : layout(std::make_unique<Layout>(viewportSize)), renderable(std::make_unique<DefaultRenderable>()) {
    renderable->setLayout(this->layout.get());
    layout->setHirarchyNode(this);
}

Node::Node(std::unique_ptr<Layout> layout, std::unique_ptr<IRenderable> renderable) : layout(std::move(layout)), renderable(std::move(renderable)) {
    renderable->setLayout(this->layout.get());
}

void Node::setParent(Node &newParent) {
    if (this->parent.has_value()) {
        auto children = &this->parent.value()->children;
        // get iterator to this node
        auto it = std::find_if(children->begin(), children->end(), [this](Node &node) {
            return &node == this;
        });
        // move this node to new parent
        newParent.children.push_back(std::move(*it));
    }
    else {
        newParent.addChild(*this);
    }
    this->parent = &newParent;
}

void Node::addChild(Node &child) {
    children.push_back(std::move(child));
}

std::vector<Layout*> Node::getLayoutChildren() {
    std::vector<Layout*> result;
    for (auto &child : children) {
        result.push_back(child.getLayout());
    }
    return result;
}

void Node::recursiveRegisterForRender(Renderer &renderer) {
    renderer.queue(renderable.get());
    for (auto &child : children) {
        child.recursiveRegisterForRender(renderer);
    }
}

}  // namespace gltk
