#include "layout.h"
#include <iostream>
#include <algorithm>
#include <cassert>

Layout::Layout(MessureVec2 size) : size(size), offset(0, 0) {}

Layout::Layout(
        Vec2 anchor, 
        MessureVec2 offset, 
        Vec2 pivot, 
        MessureVec2 size,
        ChildPlacement childPlacement,
        Overflow overflow
) : 
    anchor(anchor), 
    offset(offset), 
    pivot(pivot), 
    size(size),
    childPlacement(childPlacement),
    overflow(overflow)
{}

void Layout::resolveTransform(Vec2 parentSize, Vec2 parentPosition, bool forceSize) {
    Vec2 pivotPosition = offset.resolve(parentSize) + anchor * parentSize + parentPosition;
    Vec2 size = forceSize ? parentSize : this->size.resolve(parentSize);
    Vec2 pivotOffsetFromCenter = size / 2.0f - pivot * size;
    Vec2 centerPosition = pivotPosition + pivotOffsetFromCenter;
    resolvedTransform = Mat3::translationMatrix(centerPosition) * Mat3::scalingMatrix(size);
    resolvedPosition = pivotPosition - size * pivot;
    resolvedSize = size;
    if (childPlacement == ChildPlacement::Free) {
        for (Layout* child : hirarchyNode.value()->getLayoutChildren()) {
            child->resolveTransform(resolvedSize.value(), resolvedPosition.value());
        }
    }
    else if (childPlacement == ChildPlacement::ListStretch) {
        Vec2 currentPosition = resolvedPosition.value();
        float totalAbsoluteHeight = 0;
        float totalRelativeHeight = 0;
        for (Layout* child : hirarchyNode.value()->getLayoutChildren()) {
            if (child->size.x->isAbsolute()) {
                totalAbsoluteHeight += child->size.x->resolve(resolvedSize.value().x);
            }
            else {
                totalRelativeHeight += child->size.x->resolve(resolvedSize.value().x);
            }
        }
        for (Layout* child : hirarchyNode.value()->getLayoutChildren()) {
            Vec2 childSize = child->size.resolve(resolvedSize.value());
            if (child->size.x->isAbsolute()) {
                child->resolveTransform(resolvedSize.value(), currentPosition);
            }
            else {
                childSize.x = (resolvedSize.value().x - totalAbsoluteHeight) * childSize.x / totalRelativeHeight;
                child->resolveTransform(childSize, currentPosition, true);
            }
            currentPosition.x += childSize.x;
        }
    }
}

void Layout::resolveTransform() {
    Vec2 currentSize = resolvedSize.value_or(size.resolve(Vec2(-1, -1)));
    if (currentSize.x < 0 || currentSize.y < 0) {
        throw layout_exception("Cannot resolve size");
    }
    Vec2 currentPosition = resolvedPosition.value_or(Vec2(0, 0));
    for (Layout* child : hirarchyNode.value()->getLayoutChildren()) {
        child->resolveTransform(currentSize, currentPosition);
    }
}


Vec2 Layout::getSize() {
    return resolvedSize.value();
}

Mat3 Layout::getTransform() {
    return resolvedTransform.value();
}

void Layout::setSize(MessureVec2 size) {
    this->size = size;
}

void Layout::setHirarchyNode(IHirarchyNode* hirarchyNode) {
    this->hirarchyNode = hirarchyNode;
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

LayoutBuilder& LayoutBuilder::setAnchor(Vec2 anchor) {
    this->anchor = anchor;
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

LayoutBuilder& LayoutBuilder::setPivot(Vec2 pivot) {
    this->pivot = pivot;
    return *this;
}

LayoutBuilder& LayoutBuilder::setChildPlacement(ChildPlacement childPlacement) {
    this->childPlacement = childPlacement;
    return *this;
}

LayoutBuilder& LayoutBuilder::setOverflow(Overflow overflow) {
    this->overflow = overflow;
    return *this;
}

std::unique_ptr<Layout> LayoutBuilder::build() {
    return std::make_unique<Layout>(anchor, offset, pivot, size, childPlacement, overflow);
}
