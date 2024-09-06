#include "layout.h"
#include <cassert>
#include <iostream>

namespace gltk {


Layout::Layout(MessureVec2 size, Renderer *renderer) : size(size), renderer(renderer), offset(0, 0) {}

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
        this->renderer = parent->renderer;
    }
    if (renderable) {
        this->renderable = std::move(renderable);
    }
}

void Layout::registerForRenderRecursive() {
    if (resolvedTransform.has_value()) {
        if (renderable.has_value()) {
            BoundingBox clipRegion = BoundingBox(Vec2(0, 0), Vec2(1000000.0f, 1000000.0f));
            if (parent.has_value() && parent.value()->overflow == Overflow::Clip || parent.value()->overflow == Overflow::Scroll) {
                clipRegion = parent.value()->bounds;
            }
            renderer->queue(renderable.value().get(), resolvedTransform.value(), resolvedSize.value(), clipRegion);
        }
        for (Layout* child : children) {
            child->registerForRenderRecursive();
        }
    }
    else {
        throw layout_exception("Cannot render layout without resolved transform");
    }
}

void Layout::resolveTransform() {
    if (!parent.has_value()) {
        if (size.x->isAbsolute() && size.y->isAbsolute()) {
            resolvedSize = size.resolve(Vec2(0, 0));
            resolvedTransform = Mat3::scalingMatrix(resolvedSize.value());
            resolvedPosition = Vec2(0, 0);
            bounds = BoundingBox(Vec2(0, 0), resolvedSize.value());
        }
        else {
            throw layout_exception("Cannot resolve size");
        }
    }
    if (resolvedSize.has_value() && resolvedPosition.has_value()) {
        resolveChildTransforms(resolvedSize.value(), resolvedPosition.value(), ListDirection::Down);
    }
    else {
        throw layout_exception("resolveTransform called on unresolved non root layout");
    }
}

BoundingBox Layout::resolveTransform(Vec2 parentSize, Vec2 parentPosition, bool forceSize, ListDirection parentListDirection) {
    calculateTransform(parentSize, parentPosition, forceSize, parentListDirection);
    return resolveChildTransforms(parentSize, parentPosition, parentListDirection);
}

BoundingBox Layout::resolveChildTransforms(Vec2 parentSize, Vec2 parentPosition, ListDirection parentListDirection) {
    BoundingBox childBounds;
    if (childPlacement == ChildPlacement::Free) {
        for (Layout* child : children) {
            BoundingBox retBound = child->resolveTransform(resolvedSize.value(), resolvedPosition.value());
            childBounds.add(retBound);
        }
    }
    else if (childPlacement == ChildPlacement::ListStretch) {
        childBounds = resolveListStretchTransform();
    }
    else if (childPlacement == ChildPlacement::List) {
        childBounds = resolveListTransform();
    }

    if (Sizing::Fit == horizontalSizing) {
        resolvedSize->x = childBounds.max.x - childBounds.min.x;
    }

    BoundingBox renderBounds = getRenderableBounds(parentSize, parentPosition);

    if (!renderBounds.isZero()) {
        childBounds.add(renderBounds);
    }

    if (!childBounds.isZero()) {
        recalculateTransformFromBounds(childBounds);
    }


    bounds = BoundingBox(resolvedPosition.value(), resolvedPosition.value() + resolvedSize.value());
    if (overflow == Overflow::Scroll && scrolePosition != Vec2(0, 0)) {
        boundScrolePosition(childBounds);
        moveChildren(scrolePosition);
    }
    return bounds;
}

BoundingBox Layout::resolveListTransform() {
    Vec2 currentPosition = getListStartPossition();
    BoundingBox childBounds;
    for (Layout* child : children) {
        Vec2 childSize = child->size.resolve(resolvedSize.value());
        BoundingBox retBound;
        if (ListDirection::Down == listDirection || ListDirection::Right == listDirection) {
            retBound = child->resolveTransform(getListParentSize(childSize), currentPosition, true);
        }
        else {
            retBound = child->resolveTransform(getListParentSize(childSize), currentPosition - childSize, true);
        }
        adjustCurrentPosition(retBound.size(), currentPosition);
        childBounds.add(retBound);
    }
    return childBounds;
}

BoundingBox Layout::resolveListStretchTransform() {
    ListStrechResolver resolver(listDirection, resolvedSize.value(), resolvedPosition.value());
    std::vector<ChildData> childrenData;
    for (Layout* child : children) {
        ChildData data;
        data.size = &child->size;
        data.horizontalSizing = child->horizontalSizing;
        data.verticalSizing = child->verticalSizing;
        data.resolveTransform = [child](Vec2 parentSize, Vec2 parentPosition, bool forceSize, ListDirection parentListDirection) {
            return child->resolveTransform(parentSize, parentPosition, forceSize, parentListDirection);
        };
        childrenData.push_back(data);
    }
    return resolver.resolve(childrenData, resolvedSize.value());
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

void Layout::moveChildren(Vec2 delta) {
    for (Layout* child : children) {
        child->resolvedPosition = child->resolvedPosition.value() + delta;
        Vec2 center = child->resolvedPosition.value() + child->resolvedSize.value() / 2.0f;
        child->resolvedTransform = Mat3::translationMatrix(center) * Mat3::scalingMatrix(child->resolvedSize.value());
        child->moveChildren(delta);
    }
}

void Layout::boundScrolePosition(BoundingBox childBounds) {
    if (bounds.min.x < childBounds.min.x + scrolePosition.x) {
        scrolePosition.x = bounds.min.x - childBounds.min.x;
    }
    if (bounds.min.y < childBounds.min.y + scrolePosition.y) {
        scrolePosition.y = bounds.min.y - childBounds.min.y;
    }
    if (bounds.max.x > childBounds.max.x + scrolePosition.x) {
        scrolePosition.x = bounds.max.x - childBounds.max.x;
    }
    if (bounds.max.y > childBounds.max.y + scrolePosition.y) {
        scrolePosition.y = bounds.max.y - childBounds.max.y;
    }
    if (bounds.width() > childBounds.width()) {
        scrolePosition.x = 0;
    }
    if (bounds.height() > childBounds.height()) {
        scrolePosition.y = 0;
    }
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

void Layout::addOnScroleCallback(std::function<void(Vec2)> callback) {
    onScrollCallbacks.push_back(callback);
}

bool Layout::scrollEventRecursive(Vec2 mousePosition, Vec2 scroleDelta) {
    if (!bounds.contains(mousePosition)) {
        return false;
    }
    bool handledByChild = false;
    for (Layout* child : children) {
        handledByChild |= child->scrollEventRecursive(mousePosition, scroleDelta);
    }
    if (!handledByChild) {
        for (std::function<void(Vec2)> callback : onScrollCallbacks) {
            callback(scroleDelta);
            handledByChild = true;
        }
    }
    return handledByChild;
}

Vec2 Layout::getSize() {
    return resolvedSize.value();
}

void Layout::scroll(Vec2 delta) {
    scrolePosition += delta;
    resolveTransform();
    registerForRenderRecursive();
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


}  // namespace gltk
