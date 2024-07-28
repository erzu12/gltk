#include "layout.h"

Layout::Layout(MessureVec2 size) : size(size), offset(0, 0) {}

Layout::Layout(Layout *parent, MessureVec2 position, MessureVec2 size) : parent(parent), offset(position), size(size) {
    if (parent == nullptr) {
        throw layout_exception("Parent layout cannot be null");
    }
    parent->addChild(this);
}

Layout::Layout(Layout *parent, Vec2 anchor, MessureVec2 offset, Vec2 pivot, MessureVec2 size) : parent(parent), anchor(anchor), offset(offset), pivot(pivot), size(size) {
    if (parent == nullptr) {
        throw layout_exception("Parent layout cannot be null");
    }
    parent->addChild(this);
}

Mat3 Layout::resolveTransform() {
    if (!parent.has_value()) {
        throw layout_exception("Root layout cannot resolve transform");
    }
    else {
        Vec2 parentSize = parent.value()->resolveSize();
        Vec2 pivotPosition = offset.resolve(parentSize) + anchor * parentSize;
        Vec2 size = this->size.resolve(parentSize);
        Vec2 pivotOffsetFromCenter = size / 2.0f - pivot * size;
        Vec2 position = pivotPosition + pivotOffsetFromCenter;
        Mat3 translation = Mat3::translationMatrix(position) * Mat3::scalingMatrix(size);
        return translation;
    }
}

void Layout::addChild(Layout* child) {
    children.push_back(child);
    child->parent = this;
}

Vec2 Layout::resolveSize() {
    if (!parent.has_value()) {
        Vec2 size = this->size.resolve(Vec2(0, 0));
        if (size.x == 0 && size.y == 0) {
            throw layout_exception("Root layout must have an absolute size");
        }
        return size;
    }
    else {
        Vec2 parentSize = parent.value()->resolveSize();
        return size.resolve(parentSize);
    }
}

void Layout::setSize(MessureVec2 size) {
    this->size = size;
}

Vec2 MessureVec2::resolve(Vec2 parentSize) {
    return Vec2(x->resolve(parentSize.x), y->resolve(parentSize.y));
}

int AbsoluteMessure::resolve(int parentSize) {
    return value;
}

int RelativeMessure::resolve(int parentSize) {
    return value * parentSize;
}



