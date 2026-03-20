#include "typesetter.h"

namespace gltk {

Typesetter::Typesetter(Font *font, std::string text) : font(font), text(text) { typeset(); }

void Typesetter::setWidthLimit(float width) {
    widthLimit = width;
    typeset();
}

void Typesetter::setHorizontalAlign(HorizontalTextAlign align) {
    horizontalAlign = align;
    typeset();
}

std::vector<std::vector<Character>> Typesetter::getLines() { return lines; }
Vec2 Typesetter::getSize() {
    float width = 0;
    float height = 0;
    for (int i = 0; i < lines.size(); i++) {
        std::vector<Character> line = lines[i];
        float lineStartX = lineColumnToCoordinate(i, 0).x;
        float lineEnd = lineColumnToCoordinate(i, line.size()).x;
        float lineWidth = lineEnd - lineStartX;
        width = std::max(width, lineWidth);
        height += font->fontSize * lineHeight;
    }
    return Vec2(width + 4, height); // leave some padding for caret and selection
}

void Typesetter::placeCaret(Vec2 position) {
    std::cout << "place caret at: " << position.x << ", " << position.y << std::endl;
    caretPosition = 0;
    selectionStart = -1;
    caretPosition = coordinateToIndex(position);
}

void Typesetter::select(Vec2 toPos, TextAmount amount) {
    int pos = coordinateToIndex(toPos);
    if (selectionStart == -1) {
        selectionStart = caretPosition;
    }
    bool carretAtBegining = pos <= selectionStart;
    int selectionBegin = carretAtBegining ? pos : selectionStart;
    int selectionEnd = carretAtBegining ? selectionStart : pos;
    if (amount == TextAmount::Word) {
        selectionBegin = getPreviousWordStart(selectionBegin + 1);
        selectionEnd = getNextWordEnd(selectionEnd - 1);
    } else if (amount == TextAmount::Line) {
        int startLineIndex = indexToLineIndex(selectionBegin);
        int endLineIndex = indexToLineIndex(selectionEnd);
        selectionBegin = getLineStartIndex(startLineIndex);
        selectionEnd = getLineEndIndex(endLineIndex);
    } else if (amount == TextAmount::All) {
        selectionBegin = 0;
        selectionEnd = text.length();
    }
    caretPosition = carretAtBegining ? selectionBegin : selectionEnd;
    selectionStart = carretAtBegining ? selectionEnd : selectionBegin;
}

void Typesetter::moveCaret(bool forward, TextAmount amount, bool select) {
    int pos = caretPosition;
    if (pos >= 0) {
        if (forward) {
            if (amount == TextAmount::Character) {
                pos++;
            } else if (amount == TextAmount::Word) {
                pos = getNextWordEnd(pos);
            } else if (amount == TextAmount::Line) {
                int lineIndex = indexToLineIndex(pos);
                int newLineIndex = std::min(lineIndex + 1, (int)lineStartIndices.size() - 1);
                int charIndex = pos - lineStartIndices[lineIndex];
                charIndex = std::min(charIndex, (int)lines[newLineIndex].size() - 1);
                pos = lineStartIndices[newLineIndex] + charIndex;
            } else if (amount == TextAmount::All) {
                pos = text.length();
            }
        } else {
            if (amount == TextAmount::Character) {
                pos--;
            } else if (amount == TextAmount::Word) {
                pos = getPreviousWordStart(pos);
            } else if (amount == TextAmount::Line) {
                int lineIndex = indexToLineIndex(pos);
                int newLineIndex = std::max(0, lineIndex - 1);
                int charIndex = pos - lineStartIndices[lineIndex];
                charIndex = std::min(charIndex, (int)lines[newLineIndex].size() - 1);
                pos = lineStartIndices[newLineIndex] + charIndex;
            } else if (amount == TextAmount::All) {
                pos = 0;
            }
        }
    }
    pos = std::max(0, std::min(pos, (int)text.length()));
    if (select) {
        if (selectionStart == -1) {
            selectionStart = caretPosition;
        }
        caretPosition = pos;
    } else {
        if (selectionStart != -1) {
            int selectionBegin = std::min(selectionStart, caretPosition);
            int selectionEnd = std::max(selectionStart, caretPosition);
            caretPosition = forward ? selectionEnd : selectionBegin;
            selectionStart = -1;
        } else {
            caretPosition = pos;
        }
    }
}

void Typesetter::changeText(const std::string &newText, bool deleteText, bool forward, TextAmount amount) {
    if (selectionStart != -1) {
        int selectionBegin = std::min(selectionStart, caretPosition);
        int selectionLength = std::abs(selectionStart - caretPosition);
        text.erase(selectionBegin, selectionLength);
        caretPosition = selectionBegin;
        selectionStart = -1;
    } else if (deleteText) {
        if (forward) {
            if (amount == TextAmount::Word) {
                int endPos = getNextWordEnd(caretPosition);
                text.erase(caretPosition, endPos - caretPosition);
            } else if (caretPosition < (int)text.length()) {
                text.erase(caretPosition, 1);
            }
        } else {
            if (amount == TextAmount::Word) {
                int endPos = getPreviousWordStart(caretPosition);
                text.erase(endPos, caretPosition - endPos);
                caretPosition = endPos;
            } else if (caretPosition > 0) {
                text.erase(caretPosition - 1, 1);
                caretPosition--;
            }
        }
    } else {
        text.insert(caretPosition, newText);
        caretPosition += newText.length();
    }
    typeset();
}

void Typesetter::deselect() { selectionStart = -1; }

std::vector<LineSelection> Typesetter::getSelection() {
    if (selectionStart == -1) {
        return std::vector<LineSelection>();
    }

    std::vector<LineSelection> selections;

    int selectionBegin = std::min(selectionStart, caretPosition);
    int selectionEnd = std::max(selectionStart, caretPosition);
    int startLine = indexToLineIndex(selectionBegin);
    int endLine = indexToLineIndex(selectionEnd);

    Vec2 start = indexToCoordinate(selectionBegin);
    Vec2 end = indexToCoordinate(lineStartIndices[startLine] + lines[startLine].size());
    if (startLine == endLine) {
        end = indexToCoordinate(selectionEnd);
        selections.push_back({LineSelection{.startX = start.x, .endX = end.x, .Y = start.y}});
    }

    for (int lineIndex = startLine + 1; lineIndex < endLine; lineIndex++) {
        start = indexToCoordinate(lineStartIndices[lineIndex]);
        end = indexToCoordinate(lineStartIndices[lineIndex] + lines[lineIndex].size());
        selections.push_back({LineSelection{.startX = start.x, .endX = end.x, .Y = start.y}});
    }
    if (startLine != endLine) {
        start = indexToCoordinate(lineStartIndices[endLine]);
        end = indexToCoordinate(selectionEnd);
        selections.push_back({LineSelection{.startX = start.x, .endX = end.x, .Y = start.y}});
    }
    return selections;
}

void Typesetter::typeset() {
    std::vector<std::string> strLines = splitTextToLines(text);
    lines.clear();
    float startY = font->fontSize;
    for (std::string strLine : strLines) {
        std::vector<Character> line;
        float startX = 2;
        for (char c : strLine) {
            Glyph ch = font->characters[c];
            float xpos = startX + ch.bearing.x;
            float ypos = startY - ch.bearing.y;
            Character character = {c, ch, Vec2(xpos, ypos)};
            line.push_back(character);
            startX += (ch.advance >> 6);
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
        float lineWidth = line.back().position.x + line.back().glyph.size.x;
        float offsetX = -maxLineWidth / 2.0f;
        if (horizontalAlign == HorizontalTextAlign::Center) {
            offsetX = -lineWidth / 2.0f;
        } else if (horizontalAlign == HorizontalTextAlign::Right) {
            offsetX = maxLineWidth - lineWidth;
        }
        for (Character &character : line) {
            character.position += Vec2(offsetX, offsetY);
        }
    }
    lineStartIndices.clear();
    int index = 0;
    for (std::vector<Character> line : lines) {
        lineStartIndices.push_back(index);
        index += line.size() + 1;
    }
}

int Typesetter::coordinateToIndex(Vec2 position) {
    int lineIndex = round((position.y + font->fontSize / 2.0f) / (font->fontSize * lineHeight));
    lineIndex = std::max(0, std::min(lineIndex, (int)lines.size() - 1));
    int minDistanceIndex = 0;
    float minDistance = std::numeric_limits<float>::max();
    for (int i = 0; i <= (int)lines[lineIndex].size(); i++) {
        float charX = lineColumnToCoordinate(lineIndex, i).x;
        float distance = std::abs(position.x - charX);
        if (distance < minDistance) {
            minDistance = distance;
            minDistanceIndex = i;
        }
    }
    return lineStartIndices[lineIndex] + minDistanceIndex;
}

int Typesetter::indexToLineIndex(int index) {
    int lineIndex = 0;
    while (lineIndex < (int)lineStartIndices.size() - 1 && lineStartIndices[lineIndex + 1] <= index) {
        lineIndex++;
    }
    return lineIndex;
}

Vec2 Typesetter::indexToCoordinate(int index) {
    int lineIndex = indexToLineIndex(index);
    int charIndex = index - lineStartIndices[lineIndex];
    return lineColumnToCoordinate(lineIndex, charIndex);
}

Vec2 Typesetter::lineColumnToCoordinate(int lineIndex, int charIndex) {
    float x = 0;
    if (lines.size() == 0) {
        return Vec2(0, 0);
    }
    if (charIndex >= (int)lines[lineIndex].size()) {
        if (lines[lineIndex].size() == 0) {
            x = 0;
        } else {
            x = lines[lineIndex].back().position.x + (lines[lineIndex].back().glyph.advance >> 6);
        }
    } else {
        x = lines[lineIndex][charIndex].position.x - font->characters[lines[lineIndex][charIndex].character].bearing.x;
    }
    float y = lineIndex * font->fontSize * lineHeight;
    return Vec2(x, y);
}

int Typesetter::getLineStartIndex(int lineIndex) {
    if (lineIndex < 0 || lineIndex >= (int)lineStartIndices.size()) {
        return -1;
    }
    return lineStartIndices[lineIndex];
}

int Typesetter::getLineEndIndex(int lineIndex) {
    if (lineIndex < 0 || lineIndex >= (int)lineStartIndices.size()) {
        return -1;
    }
    if (lineIndex == (int)lineStartIndices.size() - 1) {
        return text.length();
    }
    return lineStartIndices[lineIndex + 1] - 1;
}

int Typesetter::getPreviousWordStart(int index) {
    if (index <= 1) {
        return 0;
    }
    int lineIndex = indexToLineIndex(index);
    if (index == lineStartIndices[lineIndex]) {
        return index - 1;
    }
    size_t prevSpace = text.find_last_of(" \n", index - 2);
    return prevSpace + 1;
}

int Typesetter::getNextWordEnd(int index) {
    int lineIndex = indexToLineIndex(index);
    if (lineIndex != (int)lineStartIndices.size() - 1 && index == lineStartIndices[lineIndex + 1] - 1) {
        return index + 1;
    }
    size_t nextSpace = text.find_first_of(" \n", index + 1);
    if (nextSpace == std::string::npos) {
        return text.length();
    }
    return nextSpace;
}

std::vector<std::string> Typesetter::splitTextToLines(std::string text) {
    std::string line = "";
    std::string word = "";
    std::vector<std::string> lines;
    for (char c : text) {
        if (c == ' ' || c == '\n') {
            if (!line.empty() && getLineWidht(line + word) > widthLimit) {
                lines.push_back(line);
                line = word.substr(1);
                word = " ";
            } else {
                line += word;
                word = " ";
            }
        }
        if (c == '\n') {
            lines.push_back(line);
            line = "";
            word = "";
        }
        if (c != ' ' && c != '\n') {
            word += c;
        }
    }
    if (!line.empty() && getLineWidht(line + word) > widthLimit) {
        lines.push_back(line);
        lines.push_back(word.substr(1));
    } else {
        lines.push_back(line + word);
    }
    return lines;
}

int Typesetter::getLineWidht(std::string line) {
    int width = 0;
    for (char c : line) {
        Glyph ch = font->characters[c];
        width += (ch.advance >> 6);
    }
    return width;
}

} // namespace gltk
