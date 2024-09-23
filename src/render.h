#pragma once
#include <vector>

#include "vec_math.h"

namespace gltk {

struct Style {
    Vec3 color = Vec3(1.0f);
    std::string font = "Arial";
    int fontSize = 16;
    float radius = 0.0f;
    float borderWidth = 0.0f;
    Vec3 borderColor = Vec3(0.0f);
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
