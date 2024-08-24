#pragma once
#include <vector>

#include "vec_math.h"

namespace gltk {

class IRenderable {
public:
    virtual Vec2 getSize(Vec2 LayoutBounds, bool fixedX, bool fixedY) = 0;
    virtual void render(const Mat3 &viewMatrix, Mat3 &modelMatrix, Vec2 size) = 0;
};

struct RenderData {
    Mat3 modelMatrix;
    Vec2 size;
};

class Renderer {
    std::vector<IRenderable*> renderQueueKeys;
    std::vector<RenderData> renderQueueValues;
public:
    Renderer();

    bool willRender() { return !renderQueueKeys.empty(); }

    bool render(Mat3 viewMatrix);

    void queue(IRenderable* renderable, Mat3 modelMatrix, Vec2 size);
};

}  // namespace gltk
