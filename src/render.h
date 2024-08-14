#pragma once
#include <queue>

#include "vec_math.h"

namespace gltk {

class IRenderable {
public:
    virtual void render(const Mat3 &viewMatrix, Mat3 &modelMatrix, Vec2 size) = 0;
};

class Renderer {
    std::queue<IRenderable*> renderables;
public:
    Renderer();

    void render(Mat3 viewMatrix);

    void queue(IRenderable* renderable);
};

}  // namespace gltk
