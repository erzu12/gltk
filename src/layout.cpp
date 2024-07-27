#include "layout.h"

Layout::Layout(MessureVec2 position, MessureVec2 size) : offset(position), size(size) {}

Layout::Layout(Vec2 anchor, MessureVec2 offset, Vec2 pivot, MessureVec2 size) : anchor(anchor), offset(offset), pivot(pivot), size(size) {}

Mat3 Layout::resolveTransform(Vec2 parentSize) {
    Vec2 pivotPosition = offset.resolve(parentSize) + anchor * parentSize;
    Vec2 size = this->size.resolve(parentSize);
    Vec2 pivotOffsetFromCenter = size / 2.0f - pivot * size;
    Vec2 position = pivotPosition + pivotOffsetFromCenter;
    Mat3 translation = Mat3::translationMatrix(position) * Mat3::scalingMatrix(size);
    return translation;
}

Vec2 Layout::resolveSize(Vec2 parentSize) {
    return size.resolve(parentSize);
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



