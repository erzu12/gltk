#pragma once

#include "shader.h"
#include "render.h"

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace gltk {

struct Character {
    unsigned int textureID;
    Vec2 size;
    Vec2 bearing;
    long advance;
};

enum class HorizontalTextAlign {
    Left,
    Center,
    Right,
};

enum class VerticalTextAlign {
    Top,
    Center,
    Bottom,
};

class Text : public IRenderable {
    Style style;
    std::string text;
    HorizontalTextAlign horizontalAlign;
    VerticalTextAlign verticalAlign;
    int verticalBearing = 0;
    float lineHeight = 1.0f;

    const Shader shader = Shader("assets/text.vert", "assets/text.frag");
    unsigned int VAO, VBO;

    std::map<char, Character> Characters;

    void loadCharacters(std::string font, int fontSize);
    Vec2 getRenderdSize(std::vector<std::string> lines);
    int getRenderdLineWidht(std::string line);
    float getVerticalStartPos(float inPos, float boxSize, std::vector<std::string> lines);
    float getHorizontalStartPos(float inPos, float boxSize, std::string line);
    std::vector<std::string> splitTextToLines(std::string text, float boxWidth);
public:
    Text(std::string text, Style Style, HorizontalTextAlign horizontalAlign = HorizontalTextAlign::Left, VerticalTextAlign verticalAlign = VerticalTextAlign::Top);
    ~Text();

    void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) override;
    Vec2 getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) override;
};

}
