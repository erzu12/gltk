#include "drop_down.h"
#include <layout_builder.h>
#include <renderables/box.h>
#include <renderables/text.h>

namespace gltk {

DropDown::DropDown(Scene *scene, Layout *parent, DropDownSettings inSettings) {
    this->settings = inSettings;

    if (settings.styleSheet != nullptr) {
        settings.boxStyle = settings.styleSheet->inputBackground();
        settings.textStyle = settings.styleSheet->primaryForeground();
        settings.activeColor = settings.styleSheet->inputActive().color;
        settings.hoverColor = settings.styleSheet->accentBackground().color;
    }

    auto base = LayoutBuilder(scene, parent).setSize(settings.size).build();

    auto buttonBox = LayoutBuilder(scene, base)
                         .setSize(MessureVec2(100_pct, 100_pct))
                         .setRenderable(std::make_unique<Box>(settings.boxStyle))
                         .setPadding({10, 10, 10, 10})
                         .build();

    auto buttonText =
        LayoutBuilder(scene, buttonBox)
            .setRenderable(
                std::make_unique<Text>(
                    settings.initialText, settings.textStyle, HorizontalTextAlign::Left, VerticalTextAlign::Center
                )
            )
            .setSize(MessureVec2(100_pct, 100_pct))
            .setSizing({SizingMode::Content, SizingMode::Layout})
            .setAnchor(Anchors::CenterLeft)
            .build();

    auto arrowText =
        LayoutBuilder(scene, buttonBox)
            .setRenderable(
                std::make_unique<Text>("⏷", settings.textStyle, HorizontalTextAlign::Center, VerticalTextAlign::Center)
            )
            .setSize(MessureVec2(50_px, 100_pct))
            .setSizing({SizingMode::Content, SizingMode::Layout})
            .setAnchor(Anchors::CenterRight)
            .build();

    auto dropDownBase = LayoutBuilder(scene, base)
                            .setSize(MessureVec2(100_pct, RelativeMessure(settings.options.size())))
                            .setRenderable(std::make_unique<Box>(settings.boxStyle))
                            .setOffset(MessureVec2(0_px, 5_px))
                            .setAnchor(Anchors::BottomCenter)
                            .setPivot(Anchors::TopCenter)
                            .setChildPlacement(ChildPlacement::List)
                            .setClipOverflow(false)
                            .setZIndex(1)
                            .setVisible(false)
                            .build();

    for (auto const option : settings.options) {
        auto optionBox = LayoutBuilder(scene, dropDownBase)
                             .setSize(MessureVec2(100_pct, RelativeMessure(1.0 / settings.options.size())))
                             .setPadding({0, 10, 0, 10})
                             .setRenderable(std::make_unique<Box>(settings.boxStyle))
                             .build();
        auto optionText = LayoutBuilder(scene, optionBox)
                              .setSize(MessureVec2(100_pct, 100_pct))
                              .setRenderable(
                                  std::make_unique<Text>(
                                      option, settings.textStyle, HorizontalTextAlign::Left, VerticalTextAlign::Center
                                  )
                              )
                              .build();
        optionText->addEventCallback<MouseButtonEvent>([=, this](MouseButtonEvent &event) {
            if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::PRESS) {
                dropDownBase->getPositioning().visible = false;
                buttonText->getRenderable<Text>()->setText(option);
                for (const auto &callback : changeCallbacks) {
                    callback(option);
                }
            }
        });
        optionText->addEventCallback<MouseHoverEvent>([=, this](MouseHoverEvent &event) {
            if (event.state == HoverState::ENTER) {
                optionBox->getRenderable<Box>()->getStyle()->color = settings.hoverColor;
            } else {
                optionBox->getRenderable<Box>()->getStyle()->color = settings.boxStyle.color;
            }
        });
    }

    buttonBox->addEventCallback<MouseButtonEvent>([=, this](MouseButtonEvent &event) {
        if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::PRESS) {
            dropDownBase->getPositioning().visible = !dropDownBase->getPositioning().visible;
        }
    });

    buttonBox->addEventCallback<MouseButtonEvent>([=, this](MouseButtonEvent &event) {});
}

} // namespace gltk
