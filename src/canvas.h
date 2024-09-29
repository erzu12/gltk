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
    virtual bool pointInObject(Vec2 point) = 0;
};


class PathObject : public CanvasObject {
    std::vector<Vec2> points;
    std::vector<Vec2> borderTriangles;
    const Shader shader = Shader("assets/color.vert", "assets/color.frag");
    unsigned int VAO, VBO;
    unsigned int quadVAO, quadVBO;
    unsigned int borderVAO, borderVBO, borderEBO;
    unsigned int borderIndCount;
    bool closed = false;
    bool interpolate = false;
    Style style;

    BoundingBox boundingBox;
    Vec2 pos = Vec2(0, 0);
    bool dirty = true;
public:
    PathObject(std::vector<Vec2> points, Vec2 pos, Style style, bool interpolate, bool closed = false);
    ~PathObject();

    void rotate(float angle);
    void translate(Vec2 pos);
    void scale(Vec2 scale);

    void render(Mat3 &viewMatrix) override;

    bool pointInObject(Vec2 point) override;
private:
    void regenerateGeometry();
    BoundingBox generateBorder(std::vector<Vec2> points, float width, bool closed);
    std::vector<Vec2> bezierInterpolation(std::vector<Vec2> points);
};


class Rectangle : public PathObject {
public:
    Rectangle(Vec2 pos, Vec2 size, Style style);
};


class Oval : public PathObject {
    static std::array<Vec2, 16> ovalPoints();
public:
    Oval(Vec2 pos, Vec2 size, Style style);
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
