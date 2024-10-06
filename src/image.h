#pragma once

#include "shader.h"
#include "render.h"
#include "render_objects.h"

namespace gltk {


class Image : public IRenderable {
    Style style;
    const Shader shader = Shader("assets/image.vert", "assets/image.frag");
    ImageRenderer imageRenderer;
public:
    Image(std::string path, Style style);

    Vec2 getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) override;
    void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRergion) override;
};

}
