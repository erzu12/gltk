#pragma once

#include "render_objects.h"
#include "renderables/renderable.h"
#include "shader.h"
#include "text/typesetter.h"

namespace gltk {

enum class VerticalTextAlign {
    Top,
    Center,
    Bottom,
};

class Text : public IRenderable {
    Style style;
    Color selectionColor;
    Typesetter typesetter;

    const Shader shader = Shader("assets/text.vert", "assets/text.frag");
    BoxRenderer boxRenderer;
    unsigned int VAO, VBO;

    VerticalTextAlign verticalAlign;

    static FontManager fontManager;

    Vec2 currentInPos;

    Vec2 getTextOffset(Vec2 layoutSize);

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
    void renderText(Vec2 viewSize, Vec2 pos, Vec2 size, BoundingBox clipRegion);
    void renderSelection(Vec2 viewSize, Vec2 pos, BoundingBox clipRegion);
    void renderLineSelection(float startX, float endX, float y, Vec2 viewSize, BoundingBox clipRegion);
    Vec2 getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) override;
    void setStyle(Style style) override { this->style = style; }
    Style *getStyle() override { return &style; }
    Vec2 getCaretPosition() { return typesetter.getCaretPosition() + typesetter.getSize() / 2.0f; }
    int getCaretIndex() { return typesetter.getCaretIndex(); }

    void placeCaret(Vec2 position);
    void select(Vec2 toPos, TextAmount amount = TextAmount::Character);
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
