#include "text.h"
#include "text/fonts.h"
#include <glad/glad.h>
#include <iostream>

namespace gltk {

FontManager Text::fontManager = FontManager();

Text::Text(
    std::string text,
    Style style,
    HorizontalTextAlign horizontalAlign,
    VerticalTextAlign verticalAlign,
    Color selectionColor
)
    : style(style), verticalAlign(verticalAlign), selectionColor(selectionColor),
      typesetter(
          fontManager.getFont(
              FontSettings{.font = style.font, .slant = style.slant, .weight = style.weight, .fontSize = style.fontSize}
          ),
          text
      ) {
    typesetter.setHorizontalAlign(horizontalAlign);
    this->verticalAlign = verticalAlign;
    float vertices[] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

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
    this->currentInPos = inPos;
    currentTextOffset = getTextOffset(size) + inPos;
    renderSelection(viewSize, currentTextOffset, clipRegion);
    renderText(viewSize, currentTextOffset, size, clipRegion);
}

void Text::renderText(Vec2 viewSize, Vec2 offset, Vec2 size, BoundingBox clipRegion) {
    Mat3 viewMatrix = Mat3::viewMatrix(viewSize);
    shader.use();
    shader.UniformColor("color", style.color);
    glEnable(GL_SCISSOR_TEST);
    glScissor(clipRegion.min.x, viewSize.y - clipRegion.max.y, clipRegion.width(), clipRegion.height());
    auto lines = typesetter.getLines();
    for (auto line : lines) {
        for (Character c : line) {
            Mat3 characterModelMat = Mat3::translationMatrix(c.position + offset);
            characterModelMat.scaleMatrix(c.glyph.size);

            Mat3 transform = viewMatrix * characterModelMat;
            shader.UniformMat3("transform", transform);
            shader.UniformVec2("pixelSize", size);
            glBindTexture(GL_TEXTURE_2D, c.glyph.textureID);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    glDisable(GL_SCISSOR_TEST);
}

void Text::renderSelection(Vec2 viewSize, Vec2 offset, BoundingBox clipRegion) {
    Ivec2 caretPosition = typesetter.getCaretIndex();
    if (caretPosition >= Ivec2(0, 0)) {
        Vec2 caretPos = typesetter.getCaretPosition() + offset;
        caretPos.y -= style.fontSize * CURSOR_HIGHT_OFFSET;
        caretPos.x += 0.0f;
        Mat3 caretModelMat = Mat3::translationMatrix(caretPos);
        Vec2 caretSize = Vec2(CURSOR_WIDTH, style.fontSize * CURSOR_HIGHT);
        boxRenderer.render(viewSize, caretModelMat, caretSize, clipRegion, style);
    }
    auto lines = typesetter.getLines();
    auto selection = typesetter.getSelection();
    if (selection.size() > 0) {
        for (LineSelection lineSelection : selection) {
            renderLineSelection(
                lineSelection.startX + offset.x,
                lineSelection.endX + offset.x,
                lineSelection.Y + offset.y - style.fontSize / 2.6f,
                viewSize,
                clipRegion
            );
        }
    }
}

void Text::renderLineSelection(float startX, float endX, float y, Vec2 viewSize, BoundingBox clipRegion) {
    Vec2 selectBoxPos = Vec2(lerp(startX, endX, 0.5f), y);
    Vec2 selectBoxSize = Vec2(endX - startX, style.fontSize * 1.2f);
    Mat3 selectBoxModelMat = Mat3::translationMatrix(selectBoxPos);
    boxRenderer.render(
        viewSize, selectBoxModelMat, selectBoxSize, clipRegion, Style{.color = Color(0.2, 0.2, 0.8, 1.0)}
    );
}

Vec2 Text::getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) {
    if (fixedX) {
        typesetter.setWidthLimit(LayoutSize.x);
    }
    return typesetter.getSize();
}

void Text::placeCaret(Vec2 position) {
    typesetter.placeCaret(position - currentTextOffset + Vec2(0, style.fontSize * CURSOR_HIGHT_OFFSET));
}

void Text::select(Vec2 toPos, TextAmount amount) {
    typesetter.select(toPos - currentTextOffset + Vec2(0, style.fontSize * CURSOR_HIGHT_OFFSET), amount);
}

void Text::moveCaret(bool forward, TextAmount amount, bool select) { typesetter.moveCaret(forward, amount, select); }

void Text::changeText(const std::string &newText, bool deleteText, bool forward, TextAmount amount) {
    typesetter.changeText(newText, deleteText, forward, amount);
}

void Text::deactivate() { typesetter.deactivate(); }

Vec2 Text::getTextOffset(Vec2 layoutSize) {
    Vec2 textSize = typesetter.getSize();
    Vec2 halfSizeDiff = (layoutSize - textSize) / 2.0f;
    Vec2 offset(0, 0);
    if (verticalAlign == VerticalTextAlign::Top) {
        offset.y = -halfSizeDiff.y;
    } else if (verticalAlign == VerticalTextAlign::Bottom) {
        offset.y = halfSizeDiff.y;
    }
    if (typesetter.getHorizontalAlign() == HorizontalTextAlign::Left) {
        offset.x = -halfSizeDiff.x;
    } else if (typesetter.getHorizontalAlign() == HorizontalTextAlign::Right) {
        offset.x = halfSizeDiff.x;
    }
    return offset;
}

Text::~Text() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

}; // namespace gltk
