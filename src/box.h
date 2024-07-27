#pragma once

#include "layout.h"
#include "shader.h"

namespace gltk {

class Box {
    Layout layout;
    float radius;
    const Shader shader = Shader("assets/vert.glsl", "assets/frag.glsl");
    unsigned int VAO, VBO;
public:
    Box(Layout layout, float radius = 0.0f);
    ~Box();

    void draw(Vec2 viewportSize);
};

}
