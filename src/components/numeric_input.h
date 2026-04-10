#pragma once

#include "color.h"
#include "components/edit_text.h"
#include "layout/scene.h"
#include "messure.h"
#include "renderables/text.h"
#include "text/typesetter.h"
#include "window.h"

namespace gltk {

template <typename T>
    requires std::is_arithmetic_v<T>
struct NumericInputSettings {
    T number = 0;
    T increment = 1;
    T min = std::numeric_limits<T>::lowest();
    T max = std::numeric_limits<T>::max();
    std::optional<StyleSheet> styleSheet = std::nullopt;
    MessureVec2 size = MessureVec2(AbsoluteMessure(200), AbsoluteMessure(75));
    Style textStyle = Style({.color = Color(1.0f, 1.0f, 1.0f), .font = "Arial", .fontSize = 32});
    Style boxStyle = Style({.color = Color(0.1f, 0.1f, 0.1f), .radius = 5});
    Color selectColor = Color(0.3f, 0.3f, 0.8f);
    float scrollTriggerSize = 20.0f;
    float scrollSpeed = 10.0f;
    HorizontalTextAlign horizontalAlign = HorizontalTextAlign::Left;
    VerticalTextAlign verticalAlign = VerticalTextAlign::Center;
};

template <typename T>
    requires std::is_arithmetic_v<T>
class NumericInput {
    std::vector<std::function<void(T number)>> updateCallbacks;
    std::unique_ptr<EditText> editText;
    T currentNumber;

    T stringToNumber(const std::string &text) {
        try {
            return static_cast<T>(std::stod(text));
        } catch (std::invalid_argument &) {
            std::cout << "Invalid number: " << text << std::endl;
            return 0;
        }
    }

  public:
    NumericInput(Scene *scene, Window *window, Layout *parent, NumericInputSettings<T> settings = {}) {
        EditTextSettings editTextSettings;
        editTextSettings.text = std::to_string(settings.number);
        editTextSettings.styleSheet = settings.styleSheet;
        editTextSettings.size = std::move(settings.size);
        editTextSettings.textStyle = settings.textStyle;
        editTextSettings.boxStyle = settings.boxStyle;
        editTextSettings.selectColor = settings.selectColor;
        editTextSettings.scrollTriggerSize = settings.scrollTriggerSize;
        editTextSettings.scrollSpeed = settings.scrollSpeed;
        editTextSettings.multiline = false;
        editTextSettings.horizontalAlign = settings.horizontalAlign;
        editTextSettings.verticalAlign = settings.verticalAlign;

        editText = std::make_unique<EditText>(scene, window, parent, std::move(editTextSettings));
        editText->registerLeaveCallback([this, &settings]() {
            try {
                T number = stringToNumber(editText->getText());
                if (number < settings.min) {
                    number = settings.min;
                } else if (number > settings.max) {
                    number = settings.max;
                }
                currentNumber = number;
                std::cout << "Number updated: " << currentNumber << std::endl;
                editText->setText(std::to_string(currentNumber));
                for (auto &callback : updateCallbacks) {
                    callback(currentNumber);
                }
            } catch (std::invalid_argument &) {
                editText->setText(std::to_string(currentNumber));
            }
        });
    }

    void registerEnterCallback(std::function<void()> callback) { editText->registerEnterCallback(callback); }
    void registerUpdatedCallback(std::function<void(T number)> callback) { updateCallbacks.push_back(callback); }

    void setNumber(const T &number) { editText->setText(std::to_string(number)); }
    T getNumber() {
        try {
            return static_cast<T>(std::stod(editText->getText()));
        } catch (std::invalid_argument &) {
            std::cout << "Invalid number: " << editText->getText() << std::endl;
            return 0;
        }
    }
};

} // namespace gltk
