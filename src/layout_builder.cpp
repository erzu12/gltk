//#include "layout_builder.h"

//namespace gltk {

//LayoutBuilder::LayoutBuilder(Layout* parent) : parent(parent) {}

//LayoutBuilder& LayoutBuilder::addBox(Style style) {
    //this->style = style;
    //this->box = true;
    //return *this;
//}

//LayoutBuilder& LayoutBuilder::setAnchor(Vec2 anchor) {
    //this->anchor = anchor;
    //return *this;
//}

//LayoutBuilder& LayoutBuilder::setPivot(Vec2 pivot) {
    //this->pivot = pivot;
    //return *this;
//}

//LayoutBuilder& LayoutBuilder::setMargin(Margin margin) {
    //this->margin = margin;
    //return *this;
//}

//LayoutBuilder& LayoutBuilder::setPadding(Padding padding) {
    //this->padding = padding;
    //return *this;
//}

//LayoutBuilder& LayoutBuilder::setRenderable(std::unique_ptr<IRenderable> renderable) {
    //this->renderable = std::move(renderable);
    //return *this;
//}

//LayoutBuilder& LayoutBuilder::setOffset(MessureVec2 offset) {
    //this->offset = offset;
    //return *this;
//}

//LayoutBuilder& LayoutBuilder::setSize(MessureVec2 size) {
    //this->size = size;
    //return *this;
//}

//LayoutBuilder& LayoutBuilder::setChildPlacement(ChildPlacement childPlacement) {
    //this->childPlacement = childPlacement;
    //return *this;
//}

//LayoutBuilder& LayoutBuilder::setListDirection(ListDirection listDirection) {
    //this->listDirection = listDirection;
    //return *this;
//}

//LayoutBuilder& LayoutBuilder::setSizing(Sizing horizontalSizing, Sizing verticalSizing) {
    //this->horizontalSizing = horizontalSizing;
    //this->verticalSizing = verticalSizing;
    //return *this;
//}

//LayoutBuilder& LayoutBuilder::setOverflow(Overflow overflow) {
    //this->overflow = overflow;
    //return *this;
//}

//std::unique_ptr<Layout> LayoutBuilder::build() {
    //Positioning positioning {
        //.size = size,
        //.offset = offset,
        //.anchor = anchor,
        //.pivot = pivot,
        //.margin = margin,
        //.padding = padding,
        //.childPlacement = childPlacement,
        //.listDirection = listDirection,
        //.verticalSizing = verticalSizing,
        //.horizontalSizing = horizontalSizing,
        //.overflow = overflow
    //};
    //auto layout = std::make_unique<Layout>(parent, positioning, std::move(renderable));
    //if (box) {
        //layout->setRenderable(std::make_unique<Box>(style, layout.get()));
    //}
    //return layout;
//}

//} // namespace gltk
