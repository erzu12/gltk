#pragma once

#include "shader.h"
#include "render.h"

namespace gltk {


class Box : public IRenderable {
    Vec3 color;
    float radius;
    const Shader shader = Shader("assets/box.vert", "assets/box.frag");
    unsigned int VAO, VBO;
public:
    Box(Vec3 color, float radius = 0.0f);
    ~Box();

    Vec2 getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) override;
    void render(const Mat3 &viewMatrix, Mat3 &modelMatrix, Vec2 size) override;
};

}
