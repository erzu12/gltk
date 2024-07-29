#include "layout.h"
#include <iostream>
#include <algorithm>

Layout::Layout(MessureVec2 size) : size(size), offset(0, 0) {}

Layout::Layout(std::optional<Layout*> parent, Vec2 anchor, MessureVec2 offset, Vec2 pivot, MessureVec2 size) : anchor(anchor), offset(offset), pivot(pivot), size(size) {
    if (parent.has_value()) {
        parent.value()->addChild(this);
        this->parent = parent;
    }
}

void Layout::setParent(Layout* parent) {
    if (this->parent.has_value()) {
        auto children = &this->parent.value()->children;
        children->erase(std::remove(children->begin(), children->end(), this), children->end());
    }
    parent->addChild(this);
    this->parent = parent;
}

void Layout::resolveTransform(Vec2 parentSize, Vec2 parentPosition) {
    Vec2 pivotPosition = offset.resolve(parentSize) + anchor * parentSize + parentPosition;
    Vec2 size = this->size.resolve(parentSize);
    Vec2 pivotOffsetFromCenter = size / 2.0f - pivot * size;
    Vec2 centerPosition = pivotPosition + pivotOffsetFromCenter;
    resolvedTransform = Mat3::translationMatrix(centerPosition) * Mat3::scalingMatrix(size);
    resolvedPosition = pivotPosition - size * pivot;
    resolvedSize = size;
    for (Layout* child : children) {
        child->resolveTransform(resolvedSize.value(), resolvedPosition.value());
    }
}

void Layout::resolveTransform() {
    Vec2 currentSize = resolvedSize.value_or(size.resolve(Vec2(-1, -1)));
    if (currentSize.x < 0 || currentSize.y < 0) {
        throw layout_exception("Cannot resolve size");
    }
    Vec2 currentPosition = resolvedPosition.value_or(Vec2(0, 0));
    for (Layout* child : children) {
        child->resolveTransform(currentSize, currentPosition);
    }
}

void Layout::addChild(Layout* child) {
    children.push_back(child);
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

std::unique_ptr<Layout> LayoutBuilder::build() {
    return std::make_unique<Layout>(std::nullopt, anchor, offset, pivot, size);
}
