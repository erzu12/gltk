#include "layout_builder.h"

namespace gltk {

LayoutBuilder::LayoutBuilder(Scene *scene) : scene(scene), parent(scene->getRoot()) {}
LayoutBuilder::LayoutBuilder(Scene *scene, Layout *parent) : scene(scene), parent(parent) {}

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

LayoutBuilder &LayoutBuilder::setClipOverflow(bool overflow) {
    this->clipOverflow = overflow;
    return *this;
}

LayoutBuilder &LayoutBuilder::setZIndex(int zIndex) {
    this->zIndex = zIndex;
    return *this;
}

LayoutBuilder &LayoutBuilder::setVisible(bool visible) {
    this->visible = visible;
    return *this;
}

Layout *LayoutBuilder::build() {
    if (pivot.x == -1) {
        pivot.x = anchor.x;
        pivot.y = anchor.y;
    }
    Positioning positioning{
        .size = std::move(size),
        .offset = std::move(offset),
        .anchor = anchor,
        .pivot = pivot,
        .sizing = sizing,
        .padding = padding,
        .childPlacement = childPlacement,
        .listDirection = listDirection,
        .clipOverflow = clipOverflow,
        .visible = visible,
        .zOffset = zIndex,
    };
    auto layout = std::make_unique<Layout>(std::move(positioning), std::move(renderable));
    auto layoutRef = scene->addRelativeLayout(std::move(layout), parent);
    return layoutRef;
}

} // namespace gltk
