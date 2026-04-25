#include "button.h"
#include <layout_builder.h>
#include <renderables/box.h>
#include <renderables/text.h>

namespace gltk {

Button::Button(Scene *scene, Layout *parent, ButtonSettings inSettings) {
    this->settings = inSettings;

    if (settings.styleSheet != nullptr) {
        settings.boxStyle = settings.styleSheet->inputBackground();
        settings.textStyle = settings.styleSheet->primaryForeground();
        settings.activeColor = settings.styleSheet->inputActive().color;
        settings.hoverColor = settings.styleSheet->inputHover().color;
    }

    auto buttonBox = LayoutBuilder(scene, parent)
                         .setSize(std::move(settings.size))
                         .setRenderable(std::make_unique<Box>(settings.boxStyle))
                         .build();

    auto buttonText =
        LayoutBuilder(scene, buttonBox)
            .setRenderable(
                std::make_unique<Text>(
                    settings.text, settings.textStyle, HorizontalTextAlign::Center, VerticalTextAlign::Center
                )
            )
            .setSize(MessureVec2(100_pct, 100_pct))
            .setSizing({SizingMode::Content, SizingMode::Layout})
            .setAnchor(Anchors::Center)
            .build();

    buttonBox->addEventCallback<MouseButtonEvent>([=, this](MouseButtonEvent &event) {
        if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::PRESS) {
            Style *style = buttonBox->getRenderable().value()->getStyle();
            style->color = settings.activeColor;
            for (const auto &callback : clickCallbacks) {
                callback();
            }
        } else if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::RELEASE) {
            Style *style = buttonBox->getRenderable().value()->getStyle();
            style->color = settings.boxStyle.color;
        }
    });
}

} // namespace gltk
