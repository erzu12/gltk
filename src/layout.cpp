#include "layout.h"
#include <cassert>
#include <iostream>

namespace gltk {


Layout::Layout(MessureVec2 size) : size(size), offset(0, 0) {}

Layout::Layout(
        Layout* parent,
        Vec2 anchor, 
        MessureVec2 offset, 
        Vec2 pivot, 
        MessureVec2 size,
        ChildPlacement childPlacement,
        ListDirection listDirection,
        Sizing horizontalSizing,
        Sizing verticalSizing,
        std::unique_ptr<IRenderable> renderable,
        Overflow overflow
) : 
    anchor(anchor), 
    offset(offset), 
    pivot(pivot), 
    size(size),
    childPlacement(childPlacement),
    listDirection(listDirection),
    horizontalSizing(horizontalSizing),
    verticalSizing(verticalSizing),
    overflow(overflow)
{
    if (parent) {
        parent->addChild(this);
        this->parent = parent;
    }
    if (renderable) {
        this->renderable = std::move(renderable);
    }
}

void Layout::adjustCurrentPosition(Vec2 childSize, Vec2 &currentPosition) {
    switch (listDirection) {
        case ListDirection::Down:
            currentPosition.y += childSize.y;
            break;
        case ListDirection::Right:
            currentPosition.x += childSize.x;
            break;
        case ListDirection::Left:
            currentPosition.x -= childSize.x;
            break;
        case ListDirection::Up:
            currentPosition.y -= childSize.y;
            break;
    }
}

Vec2 Layout::getListStartPossition() {
    if (ListDirection::Down == listDirection || ListDirection::Right == listDirection) {
        return resolvedPosition.value();
    }
    else {
        Vec2 currentPosition = resolvedPosition.value() + resolvedSize.value();
        return currentPosition;
    }
}

Vec2 Layout::getListParentSize(Vec2 childSize) {
    if (ListDirection::Down == listDirection || ListDirection::Up == listDirection) {
        return Vec2(resolvedSize.value().x, childSize.y);
    }
    else {
        return Vec2(childSize.x, resolvedSize.value().y);
    }
}

BoundingBox Layout::resolveListTransform() {
    Vec2 currentPosition = getListStartPossition();
    BoundingBox childBounds;
    for (Layout* child : children) {
        Vec2 childSize = child->size.resolve(resolvedSize.value());
        if (ListDirection::Down == listDirection || ListDirection::Right == listDirection) {
            BoundingBox retBound = child->resolveTransform(getListParentSize(childSize), currentPosition, true);
            childBounds.add(retBound);
        }
        else {
            BoundingBox retBound = child->resolveTransform(getListParentSize(childSize), currentPosition - childSize, true);
            childBounds.add(retBound);
        }
        adjustCurrentPosition(childSize, currentPosition);
    }
    return childBounds;
}

BoundingBox Layout::resolveListStretchTransform(Vec2 parentSize, Vec2 parentPosition) {
    Vec2 currentPosition = getListStartPossition();
    float totalAbsoluteHeight = 0;
    float totalRelativeHeight = 0;
    for (Layout* child : children) {
        float childSize = getListDirectionMessure(child->size)->resolve(getListDirectionValue(resolvedSize.value()));
        if (getListDirectionMessure(child->size)->isAbsolute()) {
            totalAbsoluteHeight += childSize;
        }
        else {
            totalRelativeHeight += childSize;
        }
    }
    
    float remainingHeight = getListDirectionValue(resolvedSize.value()) - totalAbsoluteHeight;

    BoundingBox childBounds;
    for (Layout* child : children) {
        Vec2 childSize = child->size.resolve(resolvedSize.value());
        Vec2 childPosition = currentPosition;
        if (!getListDirectionMessure(child->size)->isAbsolute()) {
            getListDirectionValue(childSize) = remainingHeight * getListDirectionValue(childSize) / totalRelativeHeight;
        }
        if (ListDirection::Up == listDirection || ListDirection::Left == listDirection) {
            childPosition -= childSize;
        }
        BoundingBox retBound = child->resolveTransform(getListParentSize(childSize), childPosition, true, listDirection);
        childBounds.add(retBound);
        adjustCurrentPosition(childSize, currentPosition);
    }
    return childBounds;
}

void Layout::calculateTransform(Vec2 parentSize, Vec2 parentPosition, bool forceSize, ListDirection parentListDirection) {
    Vec2 pivotPosition = offset.resolve(parentSize) + anchor * parentSize + parentPosition;
    Vec2 size = this->size.resolve(parentSize);
    if (forceSize) {
        if (parentListDirection == ListDirection::Down || parentListDirection == ListDirection::Up) {
            size.y = parentSize.y;
        }
        else {
            size.x = parentSize.x;
        }
    }
    Vec2 pivotOffsetFromCenter = size / 2.0f - pivot * size;
    Vec2 centerPosition = pivotPosition + pivotOffsetFromCenter;
    resolvedTransform = Mat3::translationMatrix(centerPosition) * Mat3::scalingMatrix(size);
    resolvedPosition = pivotPosition - size * pivot;
    resolvedSize = size;
}

void Layout::recalculateTransformFromBounds(BoundingBox bounds) {
    if (Sizing::Fit == horizontalSizing || Sizing::Expand == horizontalSizing) {
        resolvedSize.value().x = std::max(bounds.max.x - bounds.min.x, resolvedSize.value().x);
        resolvedPosition.value().x = std::min(bounds.min.x, resolvedPosition.value().x);
    }
    if (Sizing::Fit == verticalSizing || Sizing::Expand == verticalSizing) {
        resolvedSize.value().y = std::max(bounds.max.y - bounds.min.y, resolvedSize.value().y);
        resolvedPosition.value().y = std::min(bounds.min.y, resolvedPosition.value().y);
    }
    if (Sizing::Fit == horizontalSizing || Sizing::Shrink == horizontalSizing) {
        resolvedSize.value().x = std::min(bounds.max.x - bounds.min.x, resolvedSize.value().x);
        resolvedPosition.value().x = std::max(bounds.min.x, resolvedPosition.value().x);
    }
    if (Sizing::Fit == verticalSizing || Sizing::Shrink == verticalSizing) {
        resolvedSize.value().y = std::min(bounds.max.y - bounds.min.y, resolvedSize.value().y);
        resolvedPosition.value().y = std::max(bounds.min.y, resolvedPosition.value().y);
    }
    if (resolvedSize.value().x < 0.001f || resolvedSize.value().y < 0.001f) {
        std::cout << "WARNING: Layout was shrunk to 0 size. Layouts with Sizing Fit or Shrink should have at least one child with a fixed size" << std::endl;
    }
    resolvedTransform = Mat3::translationMatrix(resolvedPosition.value() + resolvedSize.value() / 2.0f) * Mat3::scalingMatrix(resolvedSize.value());
}

BoundingBox Layout::resolveTransform(Vec2 parentSize, Vec2 parentPosition, bool forceSize, ListDirection parentListDirection) {
    calculateTransform(parentSize, parentPosition, forceSize, parentListDirection);
    BoundingBox childBounds;
    if (childPlacement == ChildPlacement::Free) {
        for (Layout* child : children) {
            BoundingBox retBound = child->resolveTransform(resolvedSize.value(), resolvedPosition.value());
            childBounds.add(retBound);
        }
    }
    else if (childPlacement == ChildPlacement::ListStretch) {
        childBounds = resolveListStretchTransform(parentSize, parentPosition);
    }
    else if (childPlacement == ChildPlacement::List) {
        childBounds = resolveListTransform();
    }

    if (Sizing::Fit == horizontalSizing) {
        resolvedSize->x = childBounds.max.x - childBounds.min.x;
    }

    BoundingBox renderBounds = getRenderableBounds(parentSize, parentPosition);

    if (renderBounds.max.x - renderBounds.min.x > 0.001f) {
        childBounds.add(renderBounds);
    }

    if (childBounds.max.x - childBounds.min.x > 0.001f) {
        recalculateTransformFromBounds(childBounds);
    }


    bounds = BoundingBox(resolvedPosition.value(), resolvedPosition.value() + resolvedSize.value());
    return bounds;
}

BoundingBox Layout::getRenderableBounds(Vec2 parentSize, Vec2 parentPosition) {
    bool fixedX = Sizing::Fixed == horizontalSizing || Sizing::Shrink == horizontalSizing;
    bool fixedY = Sizing::Fixed == verticalSizing || Sizing::Shrink == verticalSizing;
    Vec2 renderSize = renderable.has_value() ? renderable.value()->getSize(resolvedSize.value(), fixedX, fixedY) : Vec2(0, 0);
    Vec2 pivotPosition = offset.resolve(parentSize) + anchor * parentSize + parentPosition;
    Vec2 pivotOffsetFromCenter = renderSize / 2.0f - pivot * renderSize;
    Vec2 centerPosition = pivotPosition + pivotOffsetFromCenter;
    Vec2 topLeft = pivotPosition - renderSize * pivot;
    return BoundingBox(topLeft, topLeft + renderSize);
}

void Layout::resolveTransform() {
    if (size.x->isAbsolute() && size.y->isAbsolute()) {
        resolvedSize = size.resolve(Vec2(0, 0));
        resolvedTransform = Mat3::scalingMatrix(resolvedSize.value());
        bounds = BoundingBox(Vec2(0, 0), resolvedSize.value());
    }
    else {
        throw layout_exception("Cannot resolve size");
    }
    Vec2 currentPosition = resolvedPosition.value_or(Vec2(0, 0));
    for (Layout* child : children) {
        child->resolveTransform(*resolvedSize, currentPosition);
    }
}

IMessure *Layout::getListDirectionMessure(MessureVec2 messure) {
    if (listDirection == ListDirection::Down || listDirection == ListDirection::Up) {
        return messure.y;
    }
    else {
        return messure.x;
    }
}

float &Layout::getListDirectionValue(Vec2 &value) {
    if (listDirection == ListDirection::Down || listDirection == ListDirection::Up) {
        return value.y;
    }
    else {
        return value.x;
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

void Layout::addChild(Layout *child) {
    children.push_back(child);
}

void Layout::addOnClickCallback(std::function<void()> callback) {
    onClickCallbacks.push_back(callback);
}

void Layout::clickEventRecursive(Vec2 clickPosition) {
    if (bounds.contains(clickPosition)) {
        for (Layout* child : children) {
            child->clickEventRecursive(clickPosition);
        }
        for (std::function<void()> callback : onClickCallbacks) {
            callback();
        }
    }
}

void Layout::registerForRenderRecursive(Renderer &renderer) {
    if (resolvedTransform.has_value()) {
        if (renderable.has_value()) {
            BoundingBox clipRegion = BoundingBox(Vec2(0, 0), Vec2(1000000.0f, 1000000.0f));
            if (parent.has_value() && parent.value()->overflow == Overflow::Clip || parent.value()->overflow == Overflow::Scroll) {
                clipRegion = parent.value()->bounds;
            }
            renderer.queue(renderable.value().get(), resolvedTransform.value(), resolvedSize.value(), clipRegion);
        }
        for (Layout* child : children) {
            child->registerForRenderRecursive(renderer);
        }
    }
    else {
        throw layout_exception("Cannot render layout without resolved transform");
    }
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

}  // namespace gltk
