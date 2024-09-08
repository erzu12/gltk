#include "render.h"
#include <glad/glad.h>
#include <iostream>

#include "vec_math.h"

namespace gltk {


Renderer::Renderer() {
}

bool Renderer::render(Vec2 viewSize) {
    if (!needsRender) {
        return false;
    }
    for (int i = 0; i < renderQueueKeys.size(); i++) {
        renderQueueKeys[i]->render(viewSize, renderQueueValues[i].modelMatrix, renderQueueValues[i].size, renderQueueValues[i].clipRegion);
    }
    needsRender = false;

    return true;
}

void Renderer::queue(IRenderable* renderable, Mat3 modelMatrix, Vec2 size, BoundingBox clipRegion) {
    needsRender = true;
    for (int i = 0; i < renderQueueKeys.size(); i++) {
        if (renderQueueKeys[i] == renderable) {
            renderQueueValues[i].modelMatrix = modelMatrix;
            renderQueueValues[i].size = size;
            renderQueueValues[i].clipRegion = clipRegion;
            return;
        }
    }
    renderQueueKeys.push_back(renderable);
    renderQueueValues.push_back({modelMatrix, size, clipRegion});
}

}  // namespace gltk
