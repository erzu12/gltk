#pragma once
#include <queue>

#include "vec_math.h"

namespace gltk {

class IRenderable {
public:
    virtual void render(const Mat3 &viewMatrix, Mat3 &modelMatrix, Vec2 size) = 0;
};

struct RenderData {
    IRenderable* renderable;
    Mat3 modelMatrix;
    Vec2 size;
};

class Renderer {
    std::queue<RenderData> renderQueue;
public:
    Renderer();

    bool willRender() { return !renderQueue.empty(); }

    bool render(Mat3 viewMatrix);

    void queue(IRenderable* renderable, Mat3 modelMatrix, Vec2 size);
};

}  // namespace gltk
