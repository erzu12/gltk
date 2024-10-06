#pragma once

#include "shader.h"
#include "render.h"

namespace gltk {


class Image : public IRenderable {
    Style style;
    const Shader shader = Shader("assets/image.vert", "assets/image.frag");
    unsigned int VAO, VBO;
    unsigned int texture;
    int width, height;
    Vec2 solveTriangle(float angle, float hypotenuse);
    void loadImage(std::string path);
public:
    Image(std::string path, Style style);
    ~Image();

    Vec2 getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) override;
    void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRergion) override;
};

}
