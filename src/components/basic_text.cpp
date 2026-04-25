#include "basic_text.h"
#include "layout_builder.h"
#include "renderables/text.h"

namespace gltk {

BasicText::BasicText(Scene *scene, Layout *parent, BasicTextSettings settings) {

    if (settings.styleSheet != nullptr) {
        settings.textStyle = settings.styleSheet->primaryForeground();
    }
    Vec2 ancher = Vec2(0, 0);
    if (settings.horizontalAlign == HorizontalTextAlign::Left) {
        ancher.x = 0;
    } else if (settings.horizontalAlign == HorizontalTextAlign::Center) {
        ancher.x = 0.5f;
    } else if (settings.horizontalAlign == HorizontalTextAlign::Right) {
        ancher.x = 1.0f;
    }
    if (settings.verticalAlign == VerticalTextAlign::Top) {
        ancher.y = 0;
    } else if (settings.verticalAlign == VerticalTextAlign::Center) {
        ancher.y = 0.5f;
    } else if (settings.verticalAlign == VerticalTextAlign::Bottom) {
        ancher.y = 1.0f;
    }

    auto textLayout = LayoutBuilder(scene, parent)
                          .setRenderable(
                              std::make_unique<Text>(
                                  settings.text, settings.textStyle, settings.horizontalAlign, settings.verticalAlign
                              )
                          )
                          .setSizing({SizingMode::Content, SizingMode::Content})
                          .setAnchor(Anchors::Center)
                          .build();
}
} // namespace gltk
