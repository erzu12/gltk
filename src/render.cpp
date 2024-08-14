#include "render.h"
#include <glad/glad.h>
#include <iostream>

#include "vec_math.h"

namespace gltk {


Renderer::Renderer() {
}

void Renderer::render(Mat3 viewMatrix) {
    //for (; !renderables.empty(); renderables.pop()) {
        //renderables.front()->render(viewMatrix);
    //}
}

void Renderer::queue(IRenderable* renderable) {
    // TODO: avoid redundant renderables
    renderables.push(renderable);
}

}  // namespace gltk
