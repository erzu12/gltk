#include "edit_text.h"
#include "layout_builder.h"
#include "renderables/box.h"
#include "renderables/text.h"

namespace gltk {

void calcTextOffset(Layout *textLayout, Layout *boxLayout) {
    Vec2 caretPos = textLayout->getRenderable<Text>()->getCaretPosition();
    float boxWidth =
        boxLayout->transform.Size.x - boxLayout->positioning.padding.left - boxLayout->positioning.padding.right;
    float textWidth = textLayout->getRenderable<Text>()->getSize(Vec2(0, 0), false, false).x;
    float currentOffset = textLayout->positioning.offset.x->getValue();
    if (caretPos.x + currentOffset < 0) {
        textLayout->positioning.offset.x->setValue(-caretPos.x + 2.0f);
    } else if (caretPos.x + currentOffset > boxWidth) {
        textLayout->positioning.offset.x->setValue(boxWidth - caretPos.x - 2.0f);
    }
    if (textWidth > boxWidth && textWidth + currentOffset < boxWidth) {
        textLayout->positioning.offset.x->setValue(boxWidth - textWidth);
    }
    if (textWidth < boxWidth && currentOffset < 0) {
        textLayout->positioning.offset.x->setValue(0);
    }
}

EditText::EditText(Scene *scene, Window *window, Layout *parent, EditTextSettings settings) {

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
                            settings.text, settings.textStyle, HorizontalTextAlign::Left, VerticalTextAlign::Top
                        )
                    )
                    .setSize(MessureVec2(100_pct, 100_pct))
                    .setSizing(Sizing{SizingMode::Content, SizingMode::Layout})
                    .setAnchor(Anchors::CenterLeft)
                    .setPivot(Pivot::CenterLeft)
                    .build();

    scene->addEventCallback<MouseButtonEvent>(box, [=, &settings, this](MouseButtonEvent &event) {
        textAmount = TextAmount::Character;
        // std::cout << "repeat: " << event.repeat << std::endl;
        if (event.repeat == 1) {
            textAmount = TextAmount::Word;
        } else if (event.repeat == 2) {
            textAmount = TextAmount::Line;
        } else if (event.repeat >= 3) {
            textAmount = TextAmount::All;
        }
        if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::PRESS) {
            dragging = true;
            text->getRenderable<Text>()->placeCaret(event.pos);
            if (textAmount != TextAmount::Character) {
                text->getRenderable<Text>()->select(event.pos, textAmount);
            }
        }
    });
    scene->addEventCallback<MouseMoveEvent>(box, [=, &settings, this](MouseMoveEvent &event) {
        if (dragging) {
            text->getRenderable<Text>()->select(event.pos, textAmount);
        }
    });
    window->add_mouse_move_callback([=, &settings, this](MouseMoveEvent event) {
        if (dragging) {
            Vec2 boxLocalPos = event.pos - box->transform.Position + box->transform.Size / 2.0f;
            float startOffset = text->positioning.offset.x->getValue();
            if (boxLocalPos.x < settings.scrollTriggerSize) {
                float scrollSpeed = (boxLocalPos.x - settings.scrollTriggerSize) * settings.scrollSpeed * -1.0f;
                float endOffset = 0.0f;
                float duration = std::max((endOffset - startOffset) / scrollSpeed, 0.0f);
                text->positioning.offset.x->animate(AbsoluteMessure(endOffset), duration);

            } else if (boxLocalPos.x > box->transform.Size.x - settings.scrollTriggerSize) {
                float scrollSpeed =
                    (boxLocalPos.x - box->transform.Size.x + settings.scrollTriggerSize) * settings.scrollSpeed * -1.0f;
                float endOffset = box->transform.Size.x - text->transform.Size.x - 20.0f;
                float duration = std::max((endOffset - startOffset) / scrollSpeed, 0.0f);
                text->positioning.offset.x
                    ->animate(AbsoluteMessure(endOffset), duration, [=, this](float delta, float value) {
                        std::cout << "dragging to " << value << " delta: " << delta << std::endl;
                        std::cout << "actual pos: " << text->transform.Position.x << std::endl;
                        text->getRenderable<Text>()->select(event.pos, textAmount);
                    });
            } else {
                text->positioning.offset.x->stopAnimation();
            }
        }
    });
    window->add_mouse_up_callback([=, &settings, this](auto event) {
        dragging = false;
        text->positioning.offset.x->stopAnimation();
    });
    window->add_text_input_callback([=, &settings, this](TextInputEvent event) {
        std::cout << "Text input: " << event.text << std::endl;
        text->getRenderable<Text>()->changeText(event.text);
        calcTextOffset(text, box);
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
            text->getRenderable<Text>()->select(Vec2(0, 0), TextAmount::All);
        } else if (event.key == Key::KEY_V && event.mods.isModSet(KeyModifiers::MOD_CONTROL)) {
            std::string clipboardText = window->get_clipboard();
            text->getRenderable<Text>()->changeText(clipboardText);
        } else if (event.key == Key::KEY_C && event.mods.isModSet(KeyModifiers::MOD_CONTROL)) {
            std::string selectedText = text->getRenderable<Text>()->getSelectedText();
            window->set_clipboard(selectedText);
        } else if (event.key == Key::KEY_X && event.mods.isModSet(KeyModifiers::MOD_CONTROL)) {
            std::string selectedText = text->getRenderable<Text>()->getSelectedText();
            text->getRenderable<Text>()->changeText("", true, false, TextAmount::Character);
            window->set_clipboard(selectedText);
        } else if (event.key == Key::KEY_ENTER || event.key == Key::KEY_KP_ENTER) {
            text->getRenderable<Text>()->changeText("\n", false, false, TextAmount::Character);
        }
        calcTextOffset(text, box);
    });
}

} // namespace gltk
