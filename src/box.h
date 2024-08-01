#pragma once

#include "layout.h"
#include "shader.h"
#include "render.h"

namespace gltk {


class Box : public IRenderable {
    Vec3 color;
    float radius;
    const Shader shader = Shader("assets/vert.glsl", "assets/frag.glsl");
    unsigned int VAO, VBO;
    Layout *layout;
public:
    Box(Vec3 color, float radius = 0.0f);
    ~Box();

    void setLayout(Layout *layout);

    void render(Mat3 &viewMatrix) override;
};

class ListBox : public Box {
};

}
