#pragma once

#include <lunasvg.h>

#include "render.h"
#include "render_objects.h"

namespace gltk {


class Image : public IRenderable {
    Style style;
    ImageRenderer imageRenderer;
public:
    Image(std::string path, Style style);

    Vec2 getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) override;
    void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRergion) override;
};

class SVGImage : public IRenderable {
    Style style;
    ImageRenderer imageRenderer;
    std::unique_ptr<lunasvg::Document> document;
    lunasvg::Bitmap bitmap;
public:
    SVGImage(std::string path, Style style);

    Vec2 getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) override;
    void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRergion) override;
};

}
