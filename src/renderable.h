#pragma once

#include "color.h"
#include "fontloader.h"
#include "vec_math.h"

namespace gltk {

struct Style {
    Color color = Color(1.0);
    std::string font = "Arial";
    FontSlant slant = FontSlant::Roman;
    FontWeight weight = FontWeight::Normal;
    int fontSize = 16;
    float radius = 0.0f;
    float borderWidth = 0.0f;
    Color borderColor = Color();
    float rotation = 0.0f;
};

class IRenderable {
  public:
    virtual Vec2 getSize(Vec2 LayoutBounds, bool fixedX, bool fixedY) = 0;
    virtual void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) = 0;

    virtual void setStyle(Style style) = 0;
    virtual Style *getStyle() = 0;
};

} // namespace gltk
