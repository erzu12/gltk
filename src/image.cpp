#include "image.h"
#include <glad/glad.h>
#include <iostream>

#include <stb_image.h>

namespace gltk {

Vec2 Image::getSize(Vec2 layoutSize, bool fixedX, bool fixedY) {
    Vec2 outSize = Vec2();
    if (fixedX && fixedY) {
        outSize = layoutSize;
    }
    else if (fixedX) {
        outSize = Vec2(layoutSize.x, layoutSize.x * imageRenderer.height / imageRenderer.width);
    }
    else if (fixedY) {
        outSize = Vec2(layoutSize.y * imageRenderer.width / imageRenderer.height, layoutSize.y);
    }
    return outSize;
}

Image::Image(std::string path, Style style) : style(style), imageRenderer(path) {}

void Image::render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) {
    imageRenderer.render(viewSize, modelMatrix, size, clipRegion, style);
}


};
