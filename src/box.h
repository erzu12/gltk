#pragma once

#include "layout.h"
#include "shader.h"

namespace gltk {


class Box {
    std::unique_ptr<Layout> layout;
    Vec3 color;
    float radius;
    const Shader shader = Shader("assets/vert.glsl", "assets/frag.glsl");
    unsigned int VAO, VBO;
public:
    Box(std::unique_ptr<Layout> &layout, Vec3 color, float radius = 0.0f);
    ~Box();

    void addChild(Box &child);

    void draw(Vec2 viewportSize);
};

class ListBox : public Box {
};

}
