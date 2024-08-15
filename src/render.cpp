#include "render.h"
#include <glad/glad.h>
#include <iostream>

#include "vec_math.h"

namespace gltk {


Renderer::Renderer() {
}

bool Renderer::render(Mat3 viewMatrix) {
    if (renderQueue.empty()) {
        return false;
    }
    for (; !renderQueue.empty(); renderQueue.pop()) {
        std::cout << "Rendering" << std::endl;
        RenderData renderData = renderQueue.front();
        renderData.renderable->render(viewMatrix, renderData.modelMatrix, renderData.size);
    }
    return true;
}

void Renderer::queue(IRenderable* renderable, Mat3 modelMatrix, Vec2 size) {
    // TODO: avoid redundant renderables
    renderQueue.push({renderable, modelMatrix, size}); 
}

}  // namespace gltk
