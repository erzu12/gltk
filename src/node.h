#pragma once

#include <memory>

#include "layout.h"
#include "render.h"


namespace gltk {

class DefaultRenderable : public IRenderable {
public:
    void setLayout(Layout *layout) override {};
    void render(Mat3 &viewMatrix) override {};
};

class Node : public IHirarchyNode {
public:
    Node(MessureVec2 viewportSize);
    Node(std::unique_ptr<Layout> layout, std::unique_ptr<IRenderable> renderable);
    void addChild(Node &child);
    void setParent(Node &parent);

    void recursiveRegisterForRender(Renderer &renderer);

    Layout *getLayout() { return layout.get(); }

    std::vector<Layout*> getLayoutChildren() override;

private:

    std::unique_ptr<Layout> layout;
    std::unique_ptr<IRenderable> renderable;
    std::vector<Node> children;
    std::optional<Node*> parent;
};

}  // namespace gltk
