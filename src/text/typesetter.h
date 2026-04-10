#pragma once

#include "fonts.h"

namespace gltk {

struct Character {
    char character;
    Glyph glyph;
    Vec2 position;
};

enum class HorizontalTextAlign {
    Left,
    Center,
    Right,
};

enum class TextAmount {
    Character,
    Word,
    Line,
    All,
};

struct LineSelection {
    float startX;
    float endX;
    float Y;
};

class Typesetter {
  public:
    Typesetter(Font *font, std::string text);

    void setWidthLimit(float width);
    void setHorizontalAlign(HorizontalTextAlign align);
    HorizontalTextAlign getHorizontalAlign() { return horizontalAlign; }

    void setLineHeight(float lineHeight) {
        this->lineHeight = lineHeight;
        typeset();
    }

    float getLineHeight() { return lineHeight; }

    void changeText(
        const std::string &newText,
        bool deleteText = false,
        bool forward = false,
        TextAmount amount = TextAmount::Character
    );
    void placeCaret(Vec2 position);
    void moveCaret(bool forward, TextAmount amount, bool select);
    void select(Vec2 toPos, TextAmount amount = TextAmount::Character);
    void deactivate();
    bool isActive() { return caretPosition != NO_POSITION; }

    Vec2 getCaretPosition();
    Ivec2 getCaretIndex() { return caretPosition; }

    std::vector<LineSelection> getSelection();
    std::string getSelectedText();

    std::vector<std::vector<Character>> getLines();
    Vec2 getSize();

    Font *getFont() { return font; }
    std::string getText() { return text; }

  private:
    static const Ivec2 NO_POSITION;

    void typeset();

    Ivec2 coordinateToIndex(Vec2 position);
    int xCoordinateToColumn(int lineIndex, float x);
    Vec2 indexToCoordinate(Ivec2 index);
    Ivec2 getPreviousWordStart(Ivec2 index);
    Ivec2 getNextWordEnd(Ivec2 index);
    std::vector<std::string> splitTextToLines();
    int getLineWidht(std::string line);
    Ivec2 moveCaretVertical(bool forward);
    int indexToTextIndex(Ivec2 index);
    Ivec2 textIndexToIndex(int textIndex);
    Ivec2 addToIndex(Ivec2 index, int amount);

    Font *font;
    std::vector<std::vector<Character>> lines;
    std::vector<int> lineStartIndices;
    std::string text;
    float widthLimit = std::numeric_limits<float>::max();
    float emptyLineCursorX = 0;
    HorizontalTextAlign horizontalAlign = HorizontalTextAlign::Left;
    float lineHeight = 1.2f;

    Ivec2 selectionStart = Ivec2(-1, -1);
    Ivec2 caretPosition = Ivec2(-1, -1);

    float preferredCaretX = 0;
};

} // namespace gltk
