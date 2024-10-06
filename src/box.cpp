#include "box.h"
#include <glad/glad.h>
#include <iostream>

namespace gltk {

Vec2 Box::getSize(Vec2 layoutSize, bool fixedX, bool fixedY) {
    return Vec2();
}

Box::Box(Style style) : style(style), boxRenderer() {
}

void Box::render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) {
    boxRenderer.render(viewSize, modelMatrix, size, clipRegion, style);
}

Box::~Box() {
}

};
