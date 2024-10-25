#pragma once
#include <vector>

#include "vec_math.h"
#include "color.h"
#include "fontloader.h"

namespace gltk {

struct Style {
    Color color = Color(1.0);
    std::string font = "Arial";
    FontSlant slant = FontSlant::Roman;
    FontWeight weight = FontWeight::Normal;
    int fontSize = 16;
    float radius = 0.0f;
    float borderWidth = 0.0f;
    Color borderColor = Color();
    float rotation = 0.0f;
};

class IRenderable {
public:
    virtual Vec2 getSize(Vec2 LayoutBounds, bool fixedX, bool fixedY) = 0;
    virtual void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) = 0;
};

struct RenderData {
    Mat3 modelMatrix;
    Vec2 size;
    BoundingBox clipRegion;
};

class Renderer {
    std::vector<IRenderable*> renderQueueKeys;
    std::vector<RenderData> renderQueueValues;
    bool needsRender = false;
public:
    Renderer();

    bool willRender() { return !renderQueueKeys.empty(); }

    bool render(Vec2 viewSize);

    void queue(IRenderable* renderable, Mat3 modelMatrix, Vec2 size, BoundingBox clipRegion);
};

}  // namespace gltk
