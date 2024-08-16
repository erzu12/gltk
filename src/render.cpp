#include "render.h"
#include <glad/glad.h>
#include <iostream>

#include "vec_math.h"

namespace gltk {


Renderer::Renderer() {
}

bool Renderer::render(Mat3 viewMatrix) {
    if (renderQueueKeys.empty()) {
        return false;
    }
    for (int i = 0; i < renderQueueKeys.size(); i++) {
        renderQueueKeys[i]->render(viewMatrix, renderQueueValues[i].modelMatrix, renderQueueValues[i].size);
    }
    renderQueueKeys.clear();
    renderQueueValues.clear();

    return true;
}

void Renderer::queue(IRenderable* renderable, Mat3 modelMatrix, Vec2 size) {
    for (int i = 0; i < renderQueueKeys.size(); i++) {
        if (renderQueueKeys[i] == renderable) {
            renderQueueValues[i].modelMatrix = modelMatrix;
            renderQueueValues[i].size = size;
            return;
        }
    }
    renderQueueKeys.push_back(renderable);
    renderQueueValues.push_back({modelMatrix, size});
}

}  // namespace gltk
