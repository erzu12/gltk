#include "edit_text.h"
#include "layout_builder.h"
#include "renderables/box.h"
#include "renderables/text.h"

namespace gltk {

EditText::EditText(RelativeScene *scene, Window *window, RelativeLayout *parent, EditTextSettings settings) {

    if (settings.styleSheet.has_value()) {
        settings.textStyle = settings.styleSheet->getStyle("primaryForeground");
        settings.boxStyle = settings.styleSheet->getStyle("inputBackground");
    }

    auto box = LayoutBuilder(scene, parent)
                   .setRenderable(std::make_unique<Box>(settings.boxStyle))
                   .setSize(std::move(settings.size))
                   .setPadding({
                       10,
                       10,
                       10,
                       10,
                   })
                   .setAnchor(Anchors::Center)
                   .build();

    auto text = LayoutBuilder(scene, box)
                    .setRenderable(
                        std::make_unique<Text>(
                            settings.text, settings.textStyle, HorizontalTextAlign::Left, VerticalTextAlign::Center
                        )
                    )
                    .setSize(MessureVec2(100_pct, 100_pct))
                    .setSizing(Sizing{SizingMode::Content, SizingMode::Layout})
                    .setAnchor(Anchors::CenterLeft)
                    .setPivot(Pivot::CenterLeft)
                    .build();

    scene->addEventCallback<MouseButtonEvent>(
        [=, &settings, this](MouseButtonEvent &event) {
            textAmount = TextAmount::Character;
            std::cout << "repeat: " << event.repeat << std::endl;
            if (event.repeat == 1) {
                textAmount = TextAmount::Word;
            } else if (event.repeat == 2) {
                textAmount = TextAmount::Line;
            } else if (event.repeat >= 3) {
                textAmount = TextAmount::All;
            }
            if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::PRESS) {
                dragging = true;
                dragStart = event.pos;
                if (textAmount == TextAmount::Character) {
                    text->getRenderable<Text>()->placeCaret(event.pos);
                } else {
                    text->getRenderable<Text>()->select(event.pos, event.pos, textAmount);
                }
            }
        },
        text
    );
    scene->addEventCallback<MouseMoveEvent>(
        [=, &settings, this](MouseMoveEvent &event) {
            if (dragging) {
                text->getRenderable<Text>()->select(dragStart, event.pos, textAmount);
            }
        },
        text
    );
    window->add_mouse_up_callback([&](auto event) { dragging = false; });
    window->add_text_input_callback([=, &settings, this](TextInputEvent event) {
        std::cout << "Text input: " << event.text << std::endl;
        text->getRenderable<Text>()->changeText(event.text);
    });
    window->add_key_down_callback([=, &settings, this](KeyEvent event) {
        TextAmount amount = TextAmount::Character;
        if (event.mods.isModSet(KeyModifiers::MOD_CONTROL)) {
            amount = TextAmount::Word;
        }
        bool shift = event.mods.isModSet(KeyModifiers::MOD_SHIFT);
        if (event.key == Key::KEY_BACKSPACE) {
            text->getRenderable<Text>()->changeText("", true, false, amount);
        } else if (event.key == Key::KEY_DELETE) {
            text->getRenderable<Text>()->changeText("", true, true, amount);
        } else if (event.key == Key::KEY_LEFT) {
            text->getRenderable<Text>()->moveCaret(false, amount, shift);
        } else if (event.key == Key::KEY_RIGHT) {
            text->getRenderable<Text>()->moveCaret(true, amount, shift);
        } else if (event.key == Key::KEY_UP) {
            text->getRenderable<Text>()->moveCaret(false, TextAmount::Line, shift);
        } else if (event.key == Key::KEY_DOWN) {
            text->getRenderable<Text>()->moveCaret(true, TextAmount::Line, shift);
        } else if (event.key == Key::KEY_A && event.mods.isModSet(KeyModifiers::MOD_CONTROL)) {
        } else if (event.key == Key::KEY_ENTER || event.key == Key::KEY_KP_ENTER) {
            text->getRenderable<Text>()->changeText("\n", false, false, TextAmount::Character);
        }
    });
}

} // namespace gltk
