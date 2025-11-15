#pragma once

#include "shader.h"
#include "style.h"

namespace gltk {

class BoxRenderer {
    const Shader shader = Shader("assets/box.vert", "assets/box.frag");
    unsigned int VAO, VBO;

  public:
    BoxRenderer();
    ~BoxRenderer();

    void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRergion, Style style);
};

class ImageRenderer {
    const Shader shader = Shader("assets/image.vert", "assets/image.frag");
    unsigned int VAO, VBO;
    unsigned int texture;

  public:
    int width, height = 0;
    ImageRenderer(std::string path);
    ImageRenderer();
    ~ImageRenderer();

    void updateImage(uint8_t *data, int width, int height, int nrChannels);

    void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRergion, Style style);
};

} // namespace gltk
