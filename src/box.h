#pragma once

#include "shader.h"
#include "render.h"

namespace gltk {


class Box : public IRenderable {
    Vec3 color;
    float radius;
    const Shader shader = Shader("assets/vert.glsl", "assets/frag.glsl");
    unsigned int VAO, VBO;
public:
    Box(Vec3 color, float radius = 0.0f);
    ~Box();

    void render(const Mat3 &viewMatrix, Mat3 &modelMatrix, Vec2 size) override;
};

class ListBox : public Box {
};

}
