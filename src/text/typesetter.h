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
    void deselect();

    Vec2 getCaretPosition();
    int getCaretIndex() { return caretPosition; }

    std::vector<LineSelection> getSelection();

    std::vector<std::vector<Character>> getLines();
    Vec2 getSize();

    Font *getFont() { return font; }
    std::string getText() { return text; }

  private:
    void typeset();

    int coordinateToIndex(Vec2 position);
    int xCoordinateToColumn(int lineIndex, float x);
    Vec2 indexToCoordinate(int index);
    Vec2 lineColumnToCoordinate(int lineIndex, int charIndex);
    int indexToLineIndex(int index);
    int getLineStartIndex(int lineIndex);
    int getLineEndIndex(int lineIndex);
    int getPreviousWordStart(int index);
    int getNextWordEnd(int index);
    std::vector<std::string> splitTextToLines(std::string text);
    int getLineWidht(std::string line);
    int moveCaretVertical(bool forward);

    Font *font;
    std::vector<std::vector<Character>> lines;
    std::vector<int> lineStartIndices;
    std::string text;
    float widthLimit = std::numeric_limits<float>::max();
    float emptyLineCursorX = 0;
    HorizontalTextAlign horizontalAlign = HorizontalTextAlign::Left;
    float lineHeight = 1.2f;

    int selectionStart = -1;
    int caretPosition = -1;

    float preferredCaretX = 0;
};

} // namespace gltk
