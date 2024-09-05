#include "layout_builder.h"

namespace gltk {

LayoutBuilder::LayoutBuilder(Layout* parent) : parent(parent) {}

LayoutBuilder& LayoutBuilder::setAnchor(Vec2 anchor) {
    this->anchor = anchor;
    if (pivotSet == false) {
        pivot = anchor;
    }
    return *this;
}

LayoutBuilder& LayoutBuilder::setPivot(Vec2 pivot) {
    this->pivot = pivot;
    pivotSet = true;
    return *this;
}

LayoutBuilder& LayoutBuilder::setRenderable(std::unique_ptr<IRenderable> renderable) {
    this->renderable = std::move(renderable);
    return *this;
}

LayoutBuilder& LayoutBuilder::setOffset(MessureVec2 offset) {
    this->offset = offset;
    return *this;
}

LayoutBuilder& LayoutBuilder::setSize(MessureVec2 size) {
    this->size = size;
    return *this;
}

LayoutBuilder& LayoutBuilder::setChildPlacement(ChildPlacement childPlacement) {
    this->childPlacement = childPlacement;
    return *this;
}

LayoutBuilder& LayoutBuilder::setListDirection(ListDirection listDirection) {
    this->listDirection = listDirection;
    return *this;
}

LayoutBuilder& LayoutBuilder::setSizing(Sizing horizontalSizing, Sizing verticalSizing) {
    this->horizontalSizing = horizontalSizing;
    this->verticalSizing = verticalSizing;
    return *this;
}

LayoutBuilder& LayoutBuilder::setOverflow(Overflow overflow) {
    this->overflow = overflow;
    return *this;
}

std::unique_ptr<Layout> LayoutBuilder::build() {
    return std::make_unique<Layout>(parent, anchor, offset, pivot, size, childPlacement, listDirection, horizontalSizing, verticalSizing, std::move(renderable), overflow);
}

} // namespace gltk
