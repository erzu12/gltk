#include "edit_text.h"
#include <layout_builder.h>
#include <renderables/box.h>
#include <renderables/text.h>

namespace gltk {

void calcTextOffset(Layout *textLayout, Layout *boxLayout) {
    Vec2 caretPos = textLayout->getRenderable<Text>()->getCaretPosition();
    float boxWidth = boxLayout->getTransform().Size.x - boxLayout->getPositioning().padding.left -
                     boxLayout->getPositioning().padding.right;
    float textWidth = textLayout->getRenderable<Text>()->getSize(Vec2(0, 0), false, false).x;
    float currentOffset = textLayout->getPositioning().offset.x->getValue();
    if (caretPos.x + currentOffset < 0) {
        textLayout->getPositioning().offset.x->setValue(-caretPos.x + 2.0f);
    } else if (caretPos.x + currentOffset > boxWidth) {
        textLayout->getPositioning().offset.x->setValue(boxWidth - caretPos.x - 2.0f);
    }
    if (textWidth > boxWidth && textWidth + currentOffset < boxWidth) {
        textLayout->getPositioning().offset.x->setValue(boxWidth - textWidth);
    }
    if (textWidth < boxWidth && currentOffset < 0) {
        textLayout->getPositioning().offset.x->setValue(0);
    }
}

EditText::EditText(Scene *scene, Window *window, Layout *parent, EditTextSettings inSettings) {

    if (inSettings.styleSheet != nullptr) {
        inSettings.textStyle = inSettings.styleSheet->primaryForeground();
        inSettings.boxStyle = inSettings.styleSheet->inputBackground();
    }

    this->settings = std::move(inSettings);

    box = LayoutBuilder(scene, parent)
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

    Vec2 ancher = Vec2(0, 0);
    if (settings.horizontalAlign == HorizontalTextAlign::Left) {
        ancher.x = 0;
    } else if (settings.horizontalAlign == HorizontalTextAlign::Center) {
        ancher.x = 0.5f;
    } else if (settings.horizontalAlign == HorizontalTextAlign::Right) {
        ancher.x = 1.0f;
    }

    if (settings.multiline) {
        if (settings.verticalAlign == VerticalTextAlign::Top) {
            ancher.y = 0;
        } else if (settings.verticalAlign == VerticalTextAlign::Center) {
            ancher.y = 0.5f;
        } else if (settings.verticalAlign == VerticalTextAlign::Bottom) {
            ancher.y = 1.0f;
        }
    } else {
        ancher.y = 0.5f;
    }

    SizingMode horizontalSizing = settings.multiline ? SizingMode::Layout : SizingMode::Content;

    text = LayoutBuilder(scene, box)
               .setRenderable(
                   std::make_unique<Text>(
                       settings.text, settings.textStyle, settings.horizontalAlign, settings.verticalAlign
                   )
               )
               .setSize(MessureVec2(100_pct, 100_pct))
               .setSizing(Sizing{horizontalSizing, SizingMode::Layout})
               .setAnchor(ancher)
               .build();

    box->addEventCallback<MouseButtonEvent>([this](MouseButtonEvent &event) {
        if (!text->getRenderable<Text>()->isActive()) {
            for (auto &callback : enterCallbacks) {
                callback();
            }
            if (settings.selectOnFocus) {
                text->getRenderable<Text>()->select(Vec2(0, 0), TextAmount::All);
                return;
            }
        }
        textAmount = TextAmount::Character;
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
    window->add_mouse_move_callback([this](MouseMoveEvent event) {
        if (dragging) {
            text->getRenderable<Text>()->select(event.pos, textAmount);
            Vec2 boxLocalPos = event.pos - box->getTransform().Position + box->getTransform().Size / 2.0f;
            float startOffset = text->getPositioning().offset.x->getValue();
            if (boxLocalPos.x < settings.scrollTriggerSize) {
                float scrollSpeed = (boxLocalPos.x - settings.scrollTriggerSize) * settings.scrollSpeed * -1.0f;
                float endOffset = 0.0f;
                float duration = std::max((endOffset - startOffset) / scrollSpeed, 0.0f);
                text->getPositioning().offset.x->animate(AbsoluteMessure(endOffset), duration);

            } else if (boxLocalPos.x > box->getTransform().Size.x - settings.scrollTriggerSize) {
                float scrollSpeed = (boxLocalPos.x - box->getTransform().Size.x + settings.scrollTriggerSize) *
                                    settings.scrollSpeed * -1.0f;
                float endOffset = box->getTransform().Size.x - text->getTransform().Size.x - 20.0f;
                float duration = std::max((endOffset - startOffset) / scrollSpeed, 0.0f);
                text->getPositioning().offset.x->animate(
                    AbsoluteMessure(endOffset),
                    duration,
                    [=, this](float delta, float value) { text->getRenderable<Text>()->select(event.pos, textAmount); }
                );
            } else {
                text->getPositioning().offset.x->stopAnimation();
            }
        }
    });
    window->add_mouse_up_callback([this](auto event) {
        dragging = false;
        text->getPositioning().offset.x->stopAnimation();
    });
    window->add_mouse_down_callback([this](auto event) {
        if (!box->getTransform().clipBox.contains(event.pos) && text->getRenderable<Text>()->isActive()) {
            for (auto &callback : leaveCallbacks) {
                callback();
            }
            text->getRenderable<Text>()->deactivate();
        }
    });
    window->add_text_input_callback([this](TextInputEvent event) {
        if (!text->getRenderable<Text>()->isActive()) {
            return;
        }
        text->getRenderable<Text>()->changeText(event.text);
        notifyChange(text->getRenderable<Text>()->getText());
        calcTextOffset(text, box);
    });
    window->add_key_down_callback([window, this](KeyEvent event) {
        if (!text->getRenderable<Text>()->isActive()) {
            return;
        }
        TextAmount amount = TextAmount::Character;
        if (event.mods.isModSet(KeyModifiers::MOD_CONTROL)) {
            amount = TextAmount::Word;
        }
        bool shift = event.mods.isModSet(KeyModifiers::MOD_SHIFT);
        if (event.key == Key::KEY_BACKSPACE) {
            text->getRenderable<Text>()->changeText("", true, false, amount);
            notifyChange(text->getRenderable<Text>()->getText());
        } else if (event.key == Key::KEY_DELETE) {
            text->getRenderable<Text>()->changeText("", true, true, amount);
            notifyChange(text->getRenderable<Text>()->getText());
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
            notifyChange(text->getRenderable<Text>()->getText());
        } else if (event.key == Key::KEY_C && event.mods.isModSet(KeyModifiers::MOD_CONTROL)) {
            std::string selectedText = text->getRenderable<Text>()->getSelectedText();
            window->set_clipboard(selectedText);
        } else if (event.key == Key::KEY_X && event.mods.isModSet(KeyModifiers::MOD_CONTROL)) {
            std::string selectedText = text->getRenderable<Text>()->getSelectedText();
            if (selectedText.empty()) {
                return;
            }
            text->getRenderable<Text>()->changeText("", true, false, TextAmount::Character);
            window->set_clipboard(selectedText);
            notifyChange(text->getRenderable<Text>()->getText());
        } else if (event.key == Key::KEY_ENTER || event.key == Key::KEY_KP_ENTER) {
            text->getRenderable<Text>()->changeText("\n", false, false, TextAmount::Character);
            notifyChange(text->getRenderable<Text>()->getText());
        }
        calcTextOffset(text, box);
    });
}

void EditText::notifyChange(const std::string &text) {
    for (auto &callback : changeCallbacks) {
        callback(text);
    }
}

void EditText::setText(const std::string &text) {
    bool wasActive = this->text->getRenderable<Text>()->isActive();
    this->text->getRenderable<Text>()->select(Vec2(0, 0), TextAmount::All);
    this->text->getRenderable<Text>()->changeText(text);
    notifyChange(this->text->getRenderable<Text>()->getText());
    calcTextOffset(this->text, box);
    if (!wasActive) {
        this->text->getRenderable<Text>()->deactivate();
    }
}

std::string EditText::getText() { return text->getRenderable<Text>()->getText(); }

} // namespace gltk
