#pragma once

#include "shader.h"

namespace gltk {

class Render {
    unsigned int VBO, VAO;
public:
    Render();
    void createBox(float x, float y, float w, float h);
    void drawBox(Shader shader);
};

}  // namespace gltk
