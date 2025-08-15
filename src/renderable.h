#pragma once

#include "style.h"
#include "vec_math.h"

namespace gltk {

class IRenderable {
  public:
    virtual Vec2 getSize(Vec2 LayoutBounds, bool fixedX, bool fixedY) = 0;
    virtual void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) = 0;

    virtual void setStyle(Style style) = 0;
    virtual Style *getStyle() = 0;
};

} // namespace gltk
