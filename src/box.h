#pragma once

#include "render_objects.h"
#include "renderable.h"

namespace gltk {

class Box : public IRenderable {
    Style style;
    BoxRenderer boxRenderer;

  public:
    Box(Style style);
    ~Box();

    Vec2 getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) override;
    void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRergion) override;

    void setStyle(Style style) override { this->style = style; }
    Style *getStyle() override { return &style; }
};

} // namespace gltk
