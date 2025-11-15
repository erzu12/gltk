#include "layout_builder.h"
#include "renderables/box.h"

namespace gltk {

LayoutBuilder::LayoutBuilder(RelativeScene *scene) : scene(scene), parent(scene->getRoot()) {}
LayoutBuilder::LayoutBuilder(RelativeScene *scene, RelativeLayout *parent) : scene(scene), parent(parent) {}

LayoutBuilder &LayoutBuilder::addBox(Style style) {
    this->style = style;
    this->box = true;
    return *this;
}

LayoutBuilder &LayoutBuilder::setAnchor(Vec2 anchor) {
    this->anchor = anchor;
    return *this;
}

LayoutBuilder &LayoutBuilder::setPivot(Vec2 pivot) {
    this->pivot = pivot;
    return *this;
}

LayoutBuilder &LayoutBuilder::setPadding(Padding padding) {
    this->padding = padding;
    return *this;
}

LayoutBuilder &LayoutBuilder::setRenderable(std::unique_ptr<IRenderable> renderable) {
    this->renderable = std::move(renderable);
    return *this;
}

LayoutBuilder &LayoutBuilder::setOffset(MessureVec2 offset) {
    this->offset = std::move(offset);
    return *this;
}

LayoutBuilder &LayoutBuilder::setSize(MessureVec2 size) {
    this->size = std::move(size);
    return *this;
}

LayoutBuilder &LayoutBuilder::setSizing(Sizing sizing) {
    this->sizing = sizing;
    return *this;
}

LayoutBuilder &LayoutBuilder::setChildPlacement(ChildPlacement childPlacement) {
    this->childPlacement = childPlacement;
    return *this;
}

LayoutBuilder &LayoutBuilder::setListDirection(ListDirection listDirection) {
    this->listDirection = listDirection;
    return *this;
}

LayoutBuilder &LayoutBuilder::setOverflow(Overflow overflow) {
    this->overflow = overflow;
    return *this;
}

RelativeLayout *LayoutBuilder::build() {
    Positioning positioning{
        .size = std::move(size),
        .offset = std::move(offset),
        .anchor = anchor,
        .pivot = pivot,
        .sizing = sizing,
        .padding = padding,
        .childPlacement = childPlacement,
        .listDirection = listDirection,
        .overflow = overflow
    };
    auto layout = std::make_unique<RelativeLayout>();
    layout->positioning = std::move(positioning);
    layout->renderable = std::move(renderable);
    layout->parent = parent;
    if (box) {
        layout->renderable = std::make_unique<Box>(style);
    }
    auto layoutRef = scene->addRelativeLayout(std::move(layout), parent);
    return layoutRef;
}

} // namespace gltk
