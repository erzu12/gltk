#pragma once

#include "shader.h"
#include "render.h"

namespace gltk {


class Box : public IRenderable {
    Style style;
    const Shader shader = Shader("assets/box.vert", "assets/box.frag");
    unsigned int VAO, VBO;
public:
    Box(Style style);
    ~Box();

    Vec2 getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) override;
    void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRergion) override;
};

}
