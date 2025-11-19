#include "image.h"
#include <glad/glad.h>
#include <iostream>

#include <stb_image.h>

#include "fontloader.h"

namespace gltk {

namespace {}

Vec2 Image::getSize(Vec2 layoutSize, bool fixedX, bool fixedY) {
    Vec2 outSize = Vec2(imageRenderer.width, imageRenderer.height);
    if (fixedX && fixedY) {
        outSize = layoutSize;
    } else if (fixedX) {
        outSize = Vec2(layoutSize.x, layoutSize.x * imageRenderer.height / imageRenderer.width);
    } else if (fixedY) {
        outSize = Vec2(layoutSize.y * imageRenderer.width / imageRenderer.height, layoutSize.y);
    }
    return outSize;
}

Image::Image(std::string path, Style style) : style(style), imageRenderer(path) {}

void Image::render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) {
    imageRenderer.render(viewSize, modelMatrix, size, clipRegion, style);
}

SVGImage::SVGImage(std::string path, Style style) : style(style), imageRenderer() {
    FontLoader fontLoader;
    // lunasvg::FontManager::registerMissingFontCalback([&fontLoader](const std::string &family, bool bold, bool italic)
    // {
    //     FontSlant slant = italic ? FontSlant::Italic : FontSlant::Roman;
    //     FontWeight weight = bold ? FontWeight::Bold : FontWeight::Normal;
    //     std::string path = fontLoader.getFontPath(family, slant, weight);
    //     return path;
    // });
    document = lunasvg::Document::loadFromFile(path);
    if (!document) {
        throw std::runtime_error("Failed to load SVG file");
    }
}

void SVGImage::render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) {
    if (bitmap.width() != size.x || bitmap.height() != size.y) {
        bitmap = document->renderToBitmap(size.x, size.y);
        bitmap.convertToRGBA();
        imageRenderer.updateImage(bitmap.data(), bitmap.width(), bitmap.height(), 4);
    }
    imageRenderer.render(viewSize, modelMatrix, size, clipRegion, style);
}

Vec2 SVGImage::getSize(Vec2 layoutSize, bool fixedX, bool fixedY) {
    Vec2 outSize = Vec2(document->width(), document->height());
    if (fixedX && fixedY) {
        outSize = layoutSize;
    } else if (fixedX) {
        outSize = Vec2(layoutSize.x, layoutSize.x * document->height() / document->width());
    } else if (fixedY) {
        outSize = Vec2(layoutSize.y * document->width() / document->height(), layoutSize.y);
    }
    return outSize;
}

}; // namespace gltk
