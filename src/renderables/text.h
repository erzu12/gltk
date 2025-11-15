#pragma once

#include "render_objects.h"
#include "renderables/renderable.h"
#include "shader.h"

#include <ft2build.h>
#include <map>
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

enum class TextAmount {
    Character,
    Word,
    Line,
    All,
};

class Text : public IRenderable {
    Style style;
    Color selectionColor;
    std::string text;
    HorizontalTextAlign horizontalAlign;
    VerticalTextAlign verticalAlign;
    int verticalBearing = 0;
    float lineHeight = 1.0f;
    int caretPosition = -1;
    int selectionStart = -1;
    int selectionEnd = -1;

    const Shader shader = Shader("assets/text.vert", "assets/text.frag");
    BoxRenderer boxRenderer;
    unsigned int VAO, VBO;

    float textStartHeight;
    std::vector<std::vector<float>> charPositions;
    std::vector<int> lineStartIndices;
    std::map<char, Character> Characters;

    void loadCharacters();
    Vec2 getRenderdSize(std::vector<std::string> lines);
    int getRenderdLineWidht(std::string line);
    float getVerticalStartPos(float inPos, float boxSize, std::vector<std::string> lines);
    float getHorizontalStartPos(float inPos, float boxSize, std::string line);
    int coordinateToIndex(Vec2 position);
    int indexToLineIndex(int index);
    Vec2 indexToCoordinate(int index);
    std::vector<std::string> splitTextToLines(std::string text, float boxWidth);
    int getLineStartIndex(int lineIndex);
    int getLineEndIndex(int lineIndex);
    int getPreviousWordStart(int index);
    int getNextWordEnd(int index);

  public:
    Text(
        std::string text,
        Style Style,
        HorizontalTextAlign horizontalAlign = HorizontalTextAlign::Left,
        VerticalTextAlign verticalAlign = VerticalTextAlign::Top,
        Color selectionColor = Color(0.3f, 0.3f, 0.8f)
    );
    ~Text();

    void render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) override;
    void calculateCharPositions(Vec2 viewSize, Vec2 inPos, Vec2 size, std::vector<std::string> lines);
    void renderText(Vec2 viewSize, Vec2 inPos, Vec2 size, BoundingBox clipRegion, std::vector<std::string> lines);
    void renderSelection(Vec2 viewSize, BoundingBox clipRegion);
    void renderLineSelection(float startX, float endX, float y, Vec2 viewSize, BoundingBox clipRegion);
    Vec2 getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) override;
    void setStyle(Style style) override { this->style = style; }
    Style *getStyle() override { return &style; }

    void placeCaret(Vec2 position);
    void select(Vec2 start, Vec2 end, TextAmount amount = TextAmount::Character);
    void moveCaret(bool forward, TextAmount amount, bool select);
    void changeText(
        const std::string &newText,
        bool deleteText = false,
        bool forward = false,
        TextAmount amount = TextAmount::Character
    );
    void deselect();
};
} // namespace gltk
