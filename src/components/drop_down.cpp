#include "drop_down.h"
#include "layout_builder.h"
#include "renderables/box.h"
#include "renderables/text.h"

namespace gltk {

DropDown::DropDown(Scene *scene, Layout *parent, DropDownSettings inSettings) {
    this->settings = inSettings;

    if (settings.styleSheet.has_value()) {
        settings.boxStyle = settings.styleSheet->getStyle("inputBackground");
        settings.textStyle = settings.styleSheet->getStyle("primaryForeground");
        settings.activeColor = settings.styleSheet->getStyle("inputActive").color;
        settings.hoverColor = settings.styleSheet->getStyle("inputHover").color;
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

    auto dropDownBase = LayoutBuilder(scene, base)
                            .setSize(MessureVec2(100_pct, RelativeMessure(settings.options.size())))
                            .setRenderable(std::make_unique<Box>(settings.boxStyle))
                            .setOffset(MessureVec2(0_px, 5_px))
                            .setPadding({10, 10, 10, 10})
                            .setAnchor(Anchors::BottomCenter)
                            .setPivot(Anchors::TopCenter)
                            .setChildPlacement(ChildPlacement::List)
                            .setClipOverflow(false)
                            .setZIndex(1)
                            .setVisible(false)
                            .build();

    for (auto const option : settings.options) {
        auto optionLayout = LayoutBuilder(scene, dropDownBase)
                                .setSize(MessureVec2(100_pct, RelativeMessure(1.0 / settings.options.size())))
                                .setRenderable(
                                    std::make_unique<Text>(
                                        option, settings.textStyle, HorizontalTextAlign::Left, VerticalTextAlign::Center
                                    )
                                )
                                .build();
        optionLayout->addEventCallback<MouseButtonEvent>([=, this](MouseButtonEvent &event) {
            if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::PRESS) {
                std::cout << "Option clicked: " << option << std::endl;
                dropDownBase->getPositioning().visible = false;
                buttonText->getRenderable<Text>()->setText(option);
                for (const auto &callback : changeCallbacks) {
                    callback(option);
                }
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
