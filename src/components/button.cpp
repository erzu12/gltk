#include "button.h"
#include "box.h"
#include "layout_builder.h"
#include "text.h"

namespace gltk {

Button::Button(RelativeScene *scene, RelativeLayout *parent, ButtonSettings settings) {

    if (settings.styleSheet.has_value()) {
        settings.boxStyle = settings.styleSheet->getStyle("inputBackground");
        settings.textStyle = settings.styleSheet->getStyle("primaryForeground");
        settings.activeColor = settings.styleSheet->getStyle("inputActive").color;
        settings.hoverColor = settings.styleSheet->getStyle("inputHover").color;
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
            .setAnchor(Anchors::Center)
            .build();

    scene->addEventCallback<MouseButtonEvent>(
        [=, &settings, this](MouseButtonEvent &event) {
            if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::PRESS) {
                Style *style = buttonBox->renderable.value()->getStyle();
                style->color = settings.activeColor;
                for (const auto &callback : clickCallbacks) {
                    callback();
                }
            } else if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::RELEASE) {
                Style *style = buttonBox->renderable.value()->getStyle();
                style->color = settings.boxStyle.color;
            }
        },
        buttonBox
    );
}

} // namespace gltk
