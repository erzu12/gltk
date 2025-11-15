#include "text.h"
#include <glad/glad.h>
#include <iostream>

namespace gltk {

Text::Text(
    std::string text,
    Style style,
    HorizontalTextAlign horizontalAlign,
    VerticalTextAlign verticalAlign,
    Color selectionColor
)
    : text(text), style(style), horizontalAlign(horizontalAlign), verticalAlign(verticalAlign),
      selectionColor(selectionColor) {
    float vertices[] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

    loadCharacters();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Text::render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) {
    Vec2 inPos = Vec2(modelMatrix[0][2], modelMatrix[1][2]);
    auto lines = splitTextToLines(text, size.x);
    textStartHeight = getVerticalStartPos(inPos.y, size.y, lines);
    calculateCharPositions(viewSize, inPos, size, lines);
    renderSelection(viewSize, clipRegion);
    renderText(viewSize, inPos, size, clipRegion, lines);
}

void Text::calculateCharPositions(Vec2 viewSize, Vec2 inPos, Vec2 size, std::vector<std::string> lines) {
    charPositions.clear();
    lineStartIndices.clear();
    int lineStartIndex = 0;
    for (std::string line : lines) {
        float startPosX = getHorizontalStartPos(inPos.x, size.x, line);
        charPositions.push_back(std::vector<float>());
        lineStartIndices.push_back(lineStartIndex);
        lineStartIndex += line.length() + 1;
        for (char c : line) {
            Character ch = Characters[c];

            float xpos = startPosX + ch.bearing.x;
            charPositions.back().push_back(xpos);
            startPosX += (ch.advance >> 6);
        }
        charPositions.back().push_back(startPosX);
    }
}

void Text::renderText(Vec2 viewSize, Vec2 inPos, Vec2 size, BoundingBox clipRegion, std::vector<std::string> lines) {
    Mat3 viewMatrix = Mat3::viewMatrix(viewSize);
    shader.use();
    Vec2 renderdSize = getRenderdSize(lines);
    Vec2 startPos;
    startPos.y = textStartHeight;
    shader.UniformColor("color", style.color);
    glEnable(GL_SCISSOR_TEST);
    glScissor(clipRegion.min.x, viewSize.y - clipRegion.max.y, clipRegion.width(), clipRegion.height());
    for (std::string line : lines) {
        startPos.x = getHorizontalStartPos(inPos.x, size.x, line);
        for (char c : line) {
            Character ch = Characters[c];

            float xpos = startPos.x + ch.bearing.x;
            float ypos = startPos.y - ch.bearing.y;

            float w = ch.size.x;
            float h = ch.size.y;

            Mat3 characterModelMat = Mat3::translationMatrix(Vec2(xpos, ypos));
            characterModelMat.scaleMatrix(Vec2(w, h));

            Mat3 transform = viewMatrix * characterModelMat;
            shader.UniformMat3("transform", transform);
            shader.UniformVec2("pixelSize", size);
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            startPos.x += (ch.advance >> 6);
        }
        startPos.y += style.fontSize * lineHeight;
    }
    glDisable(GL_SCISSOR_TEST);
}

void Text::renderSelection(Vec2 viewSize, BoundingBox clipRegion) {
    if (caretPosition >= 0) {
        Vec2 caretPos = indexToCoordinate(caretPosition);
        caretPos.y -= verticalBearing / 2.0f;
        caretPos.x += 1.0f;
        Mat3 caretModelMat = Mat3::translationMatrix(caretPos);
        Vec2 caretSize = Vec2(2.0f, style.fontSize);
        boxRenderer.render(viewSize, caretModelMat, caretSize, clipRegion, style);
    }
    if (selectionStart >= 0 && selectionEnd >= 0) {
        int startLineIndex = 0;
        int endLineIndex = 0;
        while (endLineIndex < (int)lineStartIndices.size() - 1 && lineStartIndices[endLineIndex + 1] <= selectionEnd) {
            endLineIndex++;
            if (lineStartIndices[startLineIndex + 1] <= selectionStart) {
                startLineIndex++;
            }
        }
        int charIndex = selectionStart - lineStartIndices[startLineIndex];
        float startX = charPositions[startLineIndex][charIndex];
        float startY = textStartHeight + startLineIndex * style.fontSize * lineHeight;
        float startLineEndX = charPositions[startLineIndex].back();
        if (startLineIndex == endLineIndex) {
            charIndex = selectionEnd - lineStartIndices[endLineIndex];
            startLineEndX = charPositions[endLineIndex][charIndex];
        }
        renderLineSelection(startX, startLineEndX, startY, viewSize, clipRegion);

        for (int lineIndex = startLineIndex + 1; lineIndex < endLineIndex; lineIndex++) {
            float lineY = textStartHeight + lineIndex * style.fontSize * lineHeight;
            float lineStartX = charPositions[lineIndex][0];
            float lineEndX = charPositions[lineIndex].back();
            renderLineSelection(lineStartX, lineEndX, lineY, viewSize, clipRegion);
        }
        if (endLineIndex != startLineIndex) {
            charIndex = selectionEnd - lineStartIndices[endLineIndex];
            float endX = charPositions[endLineIndex][charIndex];
            float endY = textStartHeight + endLineIndex * style.fontSize * lineHeight;
            float endLineStartX = charPositions[endLineIndex][0];
            renderLineSelection(endLineStartX, endX, endY, viewSize, clipRegion);
        }
    }
}

void Text::renderLineSelection(float startX, float endX, float y, Vec2 viewSize, BoundingBox clipRegion) {
    Vec2 selectBoxPos = Vec2(lerp(startX, endX, 0.5f), y - verticalBearing / 2.0f);
    Vec2 selectBoxSize = Vec2(endX - startX, style.fontSize);
    Mat3 selectBoxModelMat = Mat3::translationMatrix(selectBoxPos);
    boxRenderer.render(
        viewSize, selectBoxModelMat, selectBoxSize, clipRegion, Style{.color = Color(0.2, 0.2, 0.8, 1.0)}
    );
}

Vec2 Text::getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) {
    float width = fixedX ? LayoutSize.x : std::numeric_limits<float>::max();
    auto lines = splitTextToLines(text, width);
    return getRenderdSize(lines);
}

void Text::loadCharacters() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    FontLoader fontLoader;
    std::string fontPath = fontLoader.getFontPath(style.font, style.slant, style.weight);

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, style.fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++) {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            Vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        verticalBearing = std::max(verticalBearing, face->glyph->bitmap_top);
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

std::vector<std::string> Text::splitTextToLines(std::string text, float width) {
    std::string line = "";
    std::string word = "";
    std::vector<std::string> lines;
    for (char c : text) {
        if (c == ' ' || c == '\n') {
            if (!line.empty() && getRenderdLineWidht(line + word) > width) {
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
    if (!line.empty() && getRenderdLineWidht(line + word) > width) {
        lines.push_back(line);
        lines.push_back(word.substr(1));
    } else {
        lines.push_back(line + word);
    }
    return lines;
}

int Text::getRenderdLineWidht(std::string line) {
    int width = 0;
    for (char c : line) {
        Character ch = Characters[c];
        width += (ch.advance >> 6);
    }
    return width;
}

Vec2 Text::getRenderdSize(std::vector<std::string> lines) {
    float x = 0;
    float y = 0;
    for (std::string line : lines) {
        x = std::max(x, (float)getRenderdLineWidht(line));
        y += style.fontSize * lineHeight;
    }
    return Vec2(x + 2, y); // +2 to give space for caret
}

float Text::getHorizontalStartPos(float inPos, float boxSize, std::string line) {
    float renderdLineWidht = getRenderdLineWidht(line);
    switch (horizontalAlign) {
    case HorizontalTextAlign::Left:
        return inPos - boxSize / 2;
    case HorizontalTextAlign::Center:
        return inPos - renderdLineWidht / 2;
    case HorizontalTextAlign::Right:
        return inPos + boxSize / 2 - renderdLineWidht;
    }
    return 0;
}

float Text::getVerticalStartPos(float inPos, float boxSize, std::vector<std::string> lines) {
    Vec2 renderdSize = getRenderdSize(lines);
    float startPos = inPos;
    startPos += verticalBearing;

    switch (verticalAlign) {
    case VerticalTextAlign::Top:
        startPos -= boxSize / 2;
        break;
    case VerticalTextAlign::Center:
        startPos -= renderdSize.y / 2;
        break;
    case VerticalTextAlign::Bottom:
        startPos -= renderdSize.y - boxSize / 2;
        break;
    }
    return startPos;
}

void Text::placeCaret(Vec2 position) {
    caretPosition = 0;
    selectionStart = -1;
    selectionEnd = -1;
    caretPosition = coordinateToIndex(position);
}

void Text::select(Vec2 start, Vec2 end, TextAmount amount) {
    selectionStart = coordinateToIndex(start);
    selectionEnd = coordinateToIndex(end);
    if (selectionStart == selectionEnd && amount == TextAmount::Character) {
        placeCaret(start);
        return;
    }
    if (selectionStart > selectionEnd) {
        std::swap(selectionStart, selectionEnd);
    }
    if (amount == TextAmount::Word) {
        selectionStart = getPreviousWordStart(selectionStart + 1);
        selectionEnd = getNextWordEnd(selectionEnd - 1);
    } else if (amount == TextAmount::Line) {
        int startLineIndex = indexToLineIndex(selectionStart);
        int endLineIndex = indexToLineIndex(selectionEnd);
        selectionStart = getLineStartIndex(startLineIndex);
        selectionEnd = getLineEndIndex(endLineIndex);
    } else if (amount == TextAmount::All) {
        selectionStart = 0;
        selectionEnd = text.length();
    }
    caretPosition = -1;
}

void Text::moveCaret(bool forward, TextAmount amount, bool select) {
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
                charIndex = std::min(charIndex, (int)charPositions[newLineIndex].size() - 1);
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
                charIndex = std::min(charIndex, (int)charPositions[newLineIndex].size() - 1);
                pos = lineStartIndices[newLineIndex] + charIndex;
            } else if (amount == TextAmount::All) {
                pos = 0;
            }
        }
    }
    pos = std::max(0, std::min(pos, (int)text.length()));
    if (select) {
        if (selectionStart == -1 || selectionEnd == -1) {
            selectionStart = caretPosition;
            selectionEnd = pos;
            if (selectionStart > selectionEnd) {
                std::swap(selectionStart, selectionEnd);
            }
        } else {
            if (caretPosition == selectionStart) {
                selectionStart = pos;
            } else if (caretPosition == selectionEnd) {
                selectionEnd = pos;
            }
            if (selectionStart > selectionEnd) {
                std::swap(selectionStart, selectionEnd);
            }
        }
        caretPosition = pos;
    } else {
        if (selectionStart != -1 && selectionEnd != -1) {
            caretPosition = forward ? selectionEnd : selectionStart;
            selectionStart = -1;
            selectionEnd = -1;
        } else {
            caretPosition = pos;
        }
    }
}

void Text::changeText(const std::string &newText, bool deleteText, bool forward, TextAmount amount) {
    if (selectionStart != -1 && selectionEnd != -1) {
        text.erase(selectionStart, selectionEnd - selectionStart);
        caretPosition = selectionStart;
        selectionStart = -1;
        selectionEnd = -1;
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
}

int Text::coordinateToIndex(Vec2 position) {
    int lineIndex = round((position.y - textStartHeight + verticalBearing / 2.0f) / (style.fontSize * lineHeight));
    lineIndex = std::max(0, std::min(lineIndex, (int)charPositions.size() - 1));
    for (int i = 1; i < (int)charPositions[lineIndex].size(); i++) {
        if (position.x <= charPositions[lineIndex][i]) {
            return i + lineStartIndices[lineIndex] - 1;
        }
    }
    return lineStartIndices[lineIndex] + charPositions[lineIndex].size() - 1;
}

int Text::indexToLineIndex(int index) {
    int lineIndex = 0;
    while (lineIndex < (int)lineStartIndices.size() - 1 && lineStartIndices[lineIndex + 1] <= index) {
        lineIndex++;
    }
    return lineIndex;
}

Vec2 Text::indexToCoordinate(int index) {
    int lineIndex = indexToLineIndex(index);
    int charIndex = index - lineStartIndices[lineIndex];
    float x = charPositions[lineIndex][charIndex];
    float y = textStartHeight + lineIndex * style.fontSize * lineHeight;
    return Vec2(x, y);
}

int Text::getLineStartIndex(int lineIndex) {
    if (lineIndex < 0 || lineIndex >= (int)lineStartIndices.size()) {
        return -1;
    }
    return lineStartIndices[lineIndex];
}

int Text::getLineEndIndex(int lineIndex) {
    if (lineIndex < 0 || lineIndex >= (int)lineStartIndices.size()) {
        return -1;
    }
    if (lineIndex == (int)lineStartIndices.size() - 1) {
        return text.length();
    }
    return lineStartIndices[lineIndex + 1] - 1;
}

int Text::getPreviousWordStart(int index) {
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

int Text::getNextWordEnd(int index) {
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

Text::~Text() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

}; // namespace gltk
