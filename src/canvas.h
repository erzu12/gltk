#pragma once

#include "shader.h"
#include "render.h"

namespace gltk {


class CanvasObject {
    int z_index = 0;

    int operator<(const CanvasObject &other) {
        return z_index < other.z_index;
    }
    
public:
    virtual void render(Mat3 &viewMatrix) = 0;
};

class PathObject : public CanvasObject {
    std::vector<Vec2> points;
    const Shader shader = Shader("assets/color.vert", "assets/color.frag");
    unsigned int VAO, VBO;
    unsigned int quadVAO, quadVBO;
    bool closed = false;
    Style style;
public:
    PathObject(std::vector<Vec2> points, Style style, bool closed = false);
    ~PathObject();

    void render(Mat3 &viewMatrix) override;
};

class Canvas : public IRenderable {
    Style style;
    const Shader shader = Shader("assets/box.vert", "assets/box.frag");
    unsigned int VAO, VBO;
    std::vector<std::unique_ptr<CanvasObject>> objects;
    Vec2 size;
public:
    Canvas(Style style, Vec2 size);
    ~Canvas();

    void addObject(std::unique_ptr<CanvasObject> object);

    Vec2 getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) override;
    void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRergion) override;
};

}
