#include "typesetter.h"
#include <iostream>

namespace gltk {

const Ivec2 Typesetter::NO_POSITION = Ivec2(-1, -1);

Typesetter::Typesetter(Font *font, std::string text) : font(font), text(utf8toUtf32(text)) { typeset(); }

void Typesetter::setWidthLimit(float width) {
    if (width - 4 == widthLimit) {
        return;
    }
    widthLimit = width - 4;
    typeset();
}

void Typesetter::setHorizontalAlign(HorizontalTextAlign align) {
    horizontalAlign = align;
    typeset();
}

std::vector<std::vector<Character>> Typesetter::getLines() { return lines; }
Vec2 Typesetter::getSize() {
    float width = 0;
    for (int i = 0; i < lines.size(); i++) {
        std::vector<Character> line = lines[i];
        float lineStartX = indexToCoordinate(Ivec2(i, 0)).x;
        float lineEnd = indexToCoordinate(Ivec2(i, line.size())).x;
        float lineWidth = lineEnd - lineStartX;
        width = std::max(width, lineWidth);
    }
    float height = font->heightCap + font->fontSize * (lines.size() - 1) * lineHeight;
    return Vec2(width + 4, height); // leave some padding for caret and selection
}

void Typesetter::placeCaret(Vec2 position) {
    caretPosition = Ivec2();
    selectionStart = NO_POSITION;
    caretPosition = coordinateToIndex(position);
    preferredCaretX = indexToCoordinate(caretPosition).x;
}

void Typesetter::select(Vec2 toPos, TextAmount amount) {
    Ivec2 pos = coordinateToIndex(toPos);
    if (selectionStart == NO_POSITION) {
        selectionStart = caretPosition;
    }
    bool carretAtBegining = pos <= selectionStart;
    Ivec2 selectionBegin = carretAtBegining ? pos : selectionStart;
    Ivec2 selectionEnd = carretAtBegining ? selectionStart : pos;
    if (amount == TextAmount::Word) {
        selectionBegin = getPreviousWordStart(addToIndex(selectionBegin, 1));
        selectionEnd = getNextWordEnd(addToIndex(selectionEnd, -1));
    } else if (amount == TextAmount::Line) {
        int startLineIndex = selectionBegin.x;
        int endLineIndex = selectionEnd.x;
        selectionBegin = Ivec2(startLineIndex, 0);
        selectionEnd = Ivec2(endLineIndex, lines[endLineIndex].size());
    } else if (amount == TextAmount::All) {
        selectionBegin = Ivec2(0, 0);
        selectionEnd = Ivec2(lines.size() - 1, lines.back().size());
    }
    caretPosition = carretAtBegining ? selectionBegin : selectionEnd;
    selectionStart = carretAtBegining ? selectionEnd : selectionBegin;
    preferredCaretX = indexToCoordinate(caretPosition).x;
}

void Typesetter::moveCaret(bool forward, TextAmount amount, bool select) {
    Ivec2 pos = caretPosition;
    if (pos >= Ivec2(0, 0)) {
        if (forward) {
            if (amount == TextAmount::Character) {
                pos = addToIndex(pos, 1);
            } else if (amount == TextAmount::Word) {
                pos = getNextWordEnd(pos);
            } else if (amount == TextAmount::Line) {
                pos = moveCaretVertical(true);
            } else if (amount == TextAmount::All) {
                pos = Ivec2(lines.size() - 1, lines.back().size());
            }
        } else {
            if (amount == TextAmount::Character) {
                pos = addToIndex(pos, -1);
            } else if (amount == TextAmount::Word) {
                pos = getPreviousWordStart(pos);
            } else if (amount == TextAmount::Line) {
                pos = moveCaretVertical(false);
            } else if (amount == TextAmount::All) {
                pos = Ivec2();
            }
        }
    }
    if (select) {
        if (selectionStart == NO_POSITION) {
            selectionStart = caretPosition;
        }
        caretPosition = pos;
    } else {
        if (selectionStart != NO_POSITION) {
            Ivec2 selectionBegin = std::min(selectionStart, caretPosition);
            Ivec2 selectionEnd = std::max(selectionStart, caretPosition);
            caretPosition = forward ? selectionEnd : selectionBegin;
            preferredCaretX = indexToCoordinate(pos).x;
            selectionStart = NO_POSITION;
        } else {
            caretPosition = pos;
        }
    }
    if (amount == TextAmount::Character || amount == TextAmount::Word) {
        preferredCaretX = indexToCoordinate(caretPosition).x;
    }
}

Ivec2 Typesetter::moveCaretVertical(bool forward) {
    int lineIndex = caretPosition.x;
    int newLineIndex = forward ? std::min(lineIndex + 1, (int)lineStartIndices.size() - 1) : std::max(0, lineIndex - 1);
    int column = xCoordinateToColumn(newLineIndex, preferredCaretX);
    return Ivec2(newLineIndex, column);
}

void Typesetter::setText(const std::string &text) {
    this->text = utf8toUtf32(text);
    typeset();
    if (caretPosition != NO_POSITION) {
        caretPosition = textIndexToIndex(std::min(indexToTextIndex(caretPosition), (int)this->text.size()));
        preferredCaretX = indexToCoordinate(caretPosition).x;
    }
    if (selectionStart != NO_POSITION) {
        selectionStart = textIndexToIndex(std::min(indexToTextIndex(selectionStart), (int)this->text.size()));
    }
}

void Typesetter::changeText(const std::string &newText, bool deleteText, bool forward, TextAmount amount) {
    int carretTextIndex = indexToTextIndex(caretPosition);
    bool checkDoubleSpace = false;
    if (selectionStart != NO_POSITION) {
        Ivec2 selectionBegin = std::min(selectionStart, caretPosition);
        int selectionLength = std::abs(indexToTextIndex(selectionStart) - indexToTextIndex(caretPosition));
        text.erase(indexToTextIndex(selectionBegin), selectionLength);
        caretPosition = selectionBegin;
        carretTextIndex = indexToTextIndex(caretPosition);
        selectionStart = NO_POSITION;
        checkDoubleSpace = true;
    } else if (deleteText) {
        if (forward) {
            if (amount == TextAmount::Word) {
                Ivec2 endPos = getNextWordEnd(caretPosition);
                text.erase(indexToTextIndex(caretPosition), indexToTextIndex(endPos) - indexToTextIndex(caretPosition));
                checkDoubleSpace = true;
            } else if (caretPosition < Ivec2(lines.size() - 1, lines.back().size())) {
                text.erase(indexToTextIndex(caretPosition), 1);
            }
        } else {
            if (amount == TextAmount::Word) {
                Ivec2 endPos = getPreviousWordStart(caretPosition);
                text.erase(indexToTextIndex(endPos), indexToTextIndex(caretPosition) - indexToTextIndex(endPos));
                caretPosition = endPos;
                carretTextIndex = indexToTextIndex(caretPosition);
                checkDoubleSpace = true;
            } else if (caretPosition > Ivec2()) {
                text.erase(indexToTextIndex(caretPosition) - 1, 1);
                carretTextIndex--;
            }
        }
    }
    std::u32string newTextUtf32 = utf8toUtf32(newText);
    if (checkDoubleSpace && newTextUtf32.size() == 0) {
        if (carretTextIndex > 0 && text[carretTextIndex - 1] == ' ' &&
            (carretTextIndex >= text.size() || text[carretTextIndex] == ' ')) {
            text.erase(carretTextIndex, 1);
        }
    }
    text.insert(carretTextIndex, newTextUtf32);
    typeset();
    caretPosition = textIndexToIndex(carretTextIndex + newTextUtf32.size());
    preferredCaretX = indexToCoordinate(caretPosition).x;
}

void Typesetter::deactivate() {
    selectionStart = NO_POSITION;
    caretPosition = NO_POSITION;
}

Ivec2 Typesetter::textIndexToIndex(int textIndex) {
    int lineIndex = 0;
    while (lineIndex < lineStartIndices.size() && lineStartIndices[lineIndex] <= textIndex) {
        lineIndex++;
    }
    lineIndex--;
    int columnIndex = textIndex - lineStartIndices[lineIndex];
    return Ivec2(lineIndex, columnIndex);
}

Ivec2 Typesetter::addToIndex(Ivec2 index, int amount) {
    int textIndex = indexToTextIndex(index) + amount;
    textIndex = std::max(0, std::min((int)text.size(), textIndex));
    return textIndexToIndex(textIndex);
}

Vec2 Typesetter::getCaretPosition() {
    if (caretPosition == NO_POSITION) {
        return Vec2(0, 0);
    }
    return indexToCoordinate(caretPosition);
}

std::vector<LineSelection> Typesetter::getSelection() {
    if (selectionStart == NO_POSITION) {
        return std::vector<LineSelection>();
    }

    std::vector<LineSelection> selections;

    Ivec2 selectionBegin = std::min(selectionStart, caretPosition);
    Ivec2 selectionEnd = std::max(selectionStart, caretPosition);
    int startLine = selectionBegin.x;
    int endLine = selectionEnd.x;

    Vec2 start = indexToCoordinate(selectionBegin);
    Vec2 end = indexToCoordinate(Ivec2(startLine, lines[startLine].size()));
    if (startLine == endLine) {
        end = indexToCoordinate(selectionEnd);
    }
    selections.push_back({LineSelection{.startX = start.x, .endX = end.x, .Y = start.y}});

    for (int lineIndex = startLine + 1; lineIndex < endLine; lineIndex++) {
        start = indexToCoordinate(Ivec2(lineIndex, 0));
        end = indexToCoordinate(Ivec2(lineIndex, lines[lineIndex].size()));
        selections.push_back({LineSelection{.startX = start.x, .endX = end.x, .Y = start.y}});
    }
    if (startLine != endLine) {
        start = indexToCoordinate(Ivec2(endLine, 0));
        end = indexToCoordinate(selectionEnd);
        selections.push_back({LineSelection{.startX = start.x, .endX = end.x, .Y = start.y}});
    }
    return selections;
}

std::string Typesetter::getSelectedText() {
    if (selectionStart == NO_POSITION) {
        return "";
    }
    int selectionBegin = indexToTextIndex(std::min(selectionStart, caretPosition));
    int selectionEnd = indexToTextIndex(std::max(selectionStart, caretPosition));
    return utf32toUtf8(text.substr(selectionBegin, selectionEnd - selectionBegin));
}

void Typesetter::typeset() {
    std::vector<std::u32string> strLines = splitTextToLines();
    lines.clear();
    float startY = font->heightCap;
    for (auto strLine : strLines) {
        std::vector<Character> line;
        float startX = 2;
        for (uint32_t charCode : strLine) {
            Glyph glyph = font->getGlyph(charCode);
            float xpos = startX + glyph.bearing.x;
            float ypos = startY - glyph.bearing.y;
            Character character = {charCode, glyph, Vec2(xpos, ypos)};
            line.push_back(character);
            startX += (glyph.advance >> 6);
        }
        lines.push_back(line);
        startY += font->fontSize * lineHeight;
    }
    float maxLineWidth = getSize().x;
    float hight = getSize().y;
    float offsetY = -hight / 2.0f;
    for (std::vector<Character> &line : lines) {
        if (line.size() == 0) {
            continue;
        }
        float lineWidth = line.back().position.x + (line.back().glyph.advance >> 6);
        float offsetX = -maxLineWidth / 2.0f;
        if (horizontalAlign == HorizontalTextAlign::Center) {
            offsetX = -lineWidth / 2.0f;
        } else if (horizontalAlign == HorizontalTextAlign::Right) {
            offsetX = maxLineWidth / 2.0f - lineWidth - 2.0f;
        }
        for (Character &character : line) {
            character.position += Vec2(offsetX, offsetY);
        }
    }
    if (horizontalAlign == HorizontalTextAlign::Left) {
        emptyLineCursorX = -maxLineWidth / 2.0f + 2.0f;
    } else if (horizontalAlign == HorizontalTextAlign::Center) {
        emptyLineCursorX = 0;
    } else if (horizontalAlign == HorizontalTextAlign::Right) {
        emptyLineCursorX = maxLineWidth / 2.0f - 2.0f;
    }
}

Ivec2 Typesetter::coordinateToIndex(Vec2 position) {
    Vec2 size = getSize();
    int lineIndex = round((size.y / 2.0f + position.y - font->fontSize) / (font->fontSize * lineHeight));
    lineIndex = std::max(0, std::min(lineIndex, (int)lines.size() - 1));
    int columnIndex = xCoordinateToColumn(lineIndex, position.x);
    return Ivec2(lineIndex, columnIndex);
}

int Typesetter::xCoordinateToColumn(int lineIndex, float x) {
    int charIndex = 0;
    float minDistance = std::numeric_limits<float>::max();
    for (int i = 0; i <= (int)lines[lineIndex].size(); i++) {
        float charX = indexToCoordinate(Ivec2(lineIndex, i)).x;
        float distance = std::abs(x - charX);
        if (distance < minDistance) {
            minDistance = distance;
            charIndex = i;
        }
    }
    return charIndex;
}

Vec2 Typesetter::indexToCoordinate(Ivec2 index) {
    float x = 0;
    if (index.x < 0 || index.x >= (int)lines.size()) {
        std::cout << "line index: " << index.x << " is out of bounds, line count: " << lines.size() << std::endl;
        return Vec2(0, 0);
    }
    if (index.y >= (int)lines[index.x].size()) {
        if (lines[index.x].size() == 0) {
            x = emptyLineCursorX;
        } else {
            x = lines[index.x].back().position.x + (lines[index.x].back().glyph.advance >> 6);
        }
    } else if (index.y < 0) {
        std::cout << "column index: " << index.y << " is out of bounds" << std::endl;
        x = lines[index.x][0].position.x - font->getGlyph(lines[index.x][0].charCode).bearing.x;
    } else {
        x = lines[index.x][index.y].position.x - font->getGlyph(lines[index.x][index.y].charCode).bearing.x;
    }
    float height = font->fontSize + font->fontSize * (lines.size() - 1) * lineHeight;
    float y = font->fontSize + index.x * font->fontSize * lineHeight - height / 2.0f;
    return Vec2(x, y);
}

int Typesetter::indexToTextIndex(Ivec2 index) {
    if (index.x < 0 || index.x >= (int)lines.size()) {
        std::cout << "line index: " << index.x << " is out of bounds, line count: " << lines.size() << std::endl;
        return 0;
    }
    if (index.y < 0 || index.y > (int)lines[index.x].size()) {
        std::cout << "column index: " << index.y << " is out of bounds, line length: " << lines[index.x].size()
                  << std::endl;
        return 0;
    }
    return lineStartIndices[index.x] + index.y;
}

Ivec2 Typesetter::getPreviousWordStart(Ivec2 index) {
    int textIndex = indexToTextIndex(index);
    if (textIndex == 0) {
        return Ivec2();
    }
    size_t prevNonSpace = text.find_last_not_of(U" \n", textIndex - 1);
    size_t prevSpace = text.find_last_of(U" \n", prevNonSpace);
    return addToIndex(index, prevSpace - indexToTextIndex(index) + 1);
}

Ivec2 Typesetter::getNextWordEnd(Ivec2 index) {
    size_t nextNonSpace = text.find_first_not_of(U" \n", indexToTextIndex(index));
    size_t nextSpace = text.find_first_of(U" \n", nextNonSpace);
    if (nextSpace == std::string::npos) {
        return Ivec2(lines.size() - 1, lines.back().size());
    }
    return addToIndex(index, nextSpace - indexToTextIndex(index));
}

std::vector<std::u32string> Typesetter::splitTextToLines() {
    std::u32string line = U"";
    std::u32string word = U"";
    std::vector<std::u32string> lines;
    lineStartIndices.clear();
    lineStartIndices.push_back(0);
    for (int i = 0; i < text.size(); i++) {
        char32_t c = text[i];
        if (c != '\n') {
            word += c;
        }
        if (c == ' ' || c == '\n') {
            if (!line.empty() && getLineWidht(line + word) > widthLimit) {
                lines.push_back(line);
                lineStartIndices.push_back(i - word.size() + 1);
                line = word;
                word.clear();
            } else {
                line += word;
                word.clear();
            }
        }
        if (c == '\n') {
            lines.push_back(line);
            lineStartIndices.push_back(i + 1);
            line.clear();
            word.clear();
        }
        if (c != ' ' && c != '\n') {
            if (getLineWidht(word + c) > widthLimit) {
                if (!line.empty()) {
                    lines.push_back(line);
                    line.clear();
                    lineStartIndices.push_back(i - word.size() + 1);
                }
                lines.push_back(word);
                lineStartIndices.push_back(i + 1);
                word.clear();
            }
        }
    }
    if (!line.empty() && getLineWidht(line + word) > widthLimit) {
        lines.push_back(line);
        lineStartIndices.push_back(text.size() - word.size());
        lines.push_back(word);
    } else {
        lines.push_back(line + word);
    }
    return lines;
}

int Typesetter::getLineWidht(std::u32string line) {
    int width = 0;
    int index = 0;
    for (uint32_t c : line) {
        Glyph ch = font->getGlyph(c);
        width += (ch.advance >> 6);
    }
    return width;
}

std::u32string Typesetter::utf8toUtf32(const std::string &str) {
    std::u32string utf32;
    size_t index = 0;
    while (index < str.size()) {
        uint32_t charCode = 0;
        if ((str[index] & 0x80) == 0) {
            charCode = str[index];
            index += 1;
        } else if ((str[index] & 0xE0) == 0xC0) {
            charCode = ((str[index] & 0x1F) << 6) | (str[index + 1] & 0x3F);
            index += 2;
        } else if ((str[index] & 0xF0) == 0xE0) {
            charCode = ((str[index] & 0x0F) << 12) | ((str[index + 1] & 0x3F) << 6) | (str[index + 2] & 0x3F);
            index += 3;
        } else if ((str[index] & 0xF8) == 0xF0) {
            charCode = ((str[index] & 0x07) << 18) | ((str[index + 1] & 0x3F) << 12) | ((str[index + 2] & 0x3F) << 6) |
                       (str[index + 3] & 0x3F);
            index += 4;
        } else {
            std::cout << "Invalid UTF-8 encoding at index " << index << std::endl;
            index += 1;
        }
        utf32.push_back(charCode);
    }
    return utf32;
}

std::string Typesetter::utf32toUtf8(const std::u32string &utf32) {
    std::string utf8;
    for (uint32_t charCode : utf32) {
        if (charCode <= 0x7F) {
            utf8.push_back(static_cast<char>(charCode));
        } else if (charCode <= 0x7FF) {
            utf8.push_back(static_cast<char>(0xC0 | ((charCode >> 6) & 0x1F)));
            utf8.push_back(static_cast<char>(0x80 | (charCode & 0x3F)));
        } else if (charCode <= 0xFFFF) {
            utf8.push_back(static_cast<char>(0xE0 | ((charCode >> 12) & 0x0F)));
            utf8.push_back(static_cast<char>(0x80 | ((charCode >> 6) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | (charCode & 0x3F)));
        } else if (charCode <= 0x10FFFF) {
            utf8.push_back(static_cast<char>(0xF0 | ((charCode >> 18) & 0x07)));
            utf8.push_back(static_cast<char>(0x80 | ((charCode >> 12) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | ((charCode >> 6) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | (charCode & 0x3F)));
        } else {
            std::cout << "Invalid UTF-32 code point: " << charCode << std::endl;
        }
    }
    return utf8;
}

} // namespace gltk
