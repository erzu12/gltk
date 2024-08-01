#pragma once
#include <queue>

#include "vec_math.h"
#include "layout.h"

namespace gltk {

class IRenderable {
public:
    virtual void render(Mat3 &viewMatrix) = 0;
    virtual void setLayout(Layout *layout) = 0;
};

class Renderer {
    std::queue<IRenderable*> renderables;
public:
    Renderer();

    void render(Mat3 viewMatrix);

    void queue(IRenderable* renderable);
};

}  // namespace gltk
