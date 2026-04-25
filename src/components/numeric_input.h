#pragma once

#include <color.h>
#include <components/button.h>
#include <components/edit_text.h>
#include <layout/scene.h>
#include <layout_builder.h>
#include <messure.h>
#include <renderables/box.h>
#include <renderables/text.h>
#include <text/string_to_number.h>
#include <text/typesetter.h>
#include <window.h>

namespace gltk {

template <typename T>
    requires std::is_arithmetic_v<T>
struct NumericInputSettings {
    T number = 0;
    T increment = 1;
    T min = std::numeric_limits<T>::lowest();
    T max = std::numeric_limits<T>::max();
    std::shared_ptr<StyleSheet> styleSheet = nullptr;
    MessureVec2 size = MessureVec2(AbsoluteMessure(300), AbsoluteMessure(75));
    Style textStyle = Style({.color = Color(1.0f, 1.0f, 1.0f), .font = "Arial", .fontSize = 32});
    Style boxStyle = Style({.color = Color(0.3f, 0.3f, 0.3f), .radius = 5});
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
    std::unique_ptr<Button> plusButton;
    std::unique_ptr<Button> minusButton;
    T currentNumber;

  public:
    NumericInput(Scene *scene, Window *window, Layout *parent, NumericInputSettings<T> settings = {}) {
        EditTextSettings editTextSettings;
        currentNumber = settings.number;

        if (settings.styleSheet != nullptr) {
            settings.boxStyle = settings.styleSheet->inputBackground();
            settings.textStyle = settings.styleSheet->primaryForeground();
        }

        editTextSettings.text = stringTools::numberToString(settings.number);
        editTextSettings.styleSheet = settings.styleSheet;
        editTextSettings.size = MessureVec2(100_pct, 100_pct);
        editTextSettings.textStyle = settings.textStyle;
        editTextSettings.boxStyle = Style({.color = Color::transparent()});
        editTextSettings.selectColor = settings.selectColor;
        editTextSettings.scrollTriggerSize = settings.scrollTriggerSize;
        editTextSettings.scrollSpeed = settings.scrollSpeed;
        editTextSettings.multiline = false;
        editTextSettings.selectOnFocus = true;
        editTextSettings.horizontalAlign = settings.horizontalAlign;
        editTextSettings.verticalAlign = settings.verticalAlign;

        Layout *boxLayout = LayoutBuilder(scene, parent)
                                .setRenderable(std::make_unique<Box>(settings.boxStyle))
                                .setSize(std::move(settings.size))
                                .setPadding({0, 10, 0, 0})
                                .setChildPlacement(ChildPlacement::ListStretch)
                                .setListDirection(ListDirection::Right)
                                .setAnchor(Anchors::Center)
                                .build();
        editText = std::make_unique<EditText>(scene, window, boxLayout, std::move(editTextSettings));
        minusButton = std::make_unique<Button>(
            scene,
            boxLayout,
            ButtonSettings{
                .text = "-",
                .size = MessureVec2(30_px, 50_px),
                .boxStyle = settings.boxStyle,
                .textStyle =
                    {.color = settings.textStyle.color,
                     .font = "Courier",
                     .weight = FontWeight::Bold,
                     .fontSize = settings.textStyle.fontSize}
            }
        );
        minusButton->registerClickCallback([this, &settings]() {
            T newValue = currentNumber - settings.increment;
            if (newValue < settings.min) {
                newValue = settings.min;
            }
            currentNumber = newValue;
            std::cout << "Number updated: " << currentNumber << std::endl;
            editText->setText(stringTools::numberToString(currentNumber));
            for (auto &callback : updateCallbacks) {
                callback(currentNumber);
            }
        });
        plusButton = std::make_unique<Button>(
            scene,
            boxLayout,
            ButtonSettings{
                .text = "+",
                .size = MessureVec2(30_px, 50_px),
                .boxStyle = settings.boxStyle,
                .textStyle =
                    {.color = settings.textStyle.color,
                     .font = "Courier",
                     .weight = FontWeight::Bold,
                     .fontSize = settings.textStyle.fontSize}
            }
        );
        plusButton->registerClickCallback([this, &settings]() {
            T newValue = currentNumber + settings.increment;
            if (newValue > settings.max) {
                newValue = settings.max;
            }
            currentNumber = newValue;
            std::cout << "Number updated: " << currentNumber << std::endl;
            editText->setText(stringTools::numberToString(currentNumber));
            for (auto &callback : updateCallbacks) {
                callback(currentNumber);
            }
        });
        editText->registerLeaveCallback([this, &settings]() {
            try {
                T number = stringTools::numericExpressionToNumber<T>(editText->getText());
                if (number < settings.min) {
                    number = settings.min;
                } else if (number > settings.max) {
                    number = settings.max;
                }
                currentNumber = number;
                std::cout << "Number updated: " << currentNumber << std::endl;
                editText->setText(stringTools::numberToString(currentNumber));
                for (auto &callback : updateCallbacks) {
                    callback(currentNumber);
                }
            } catch (std::invalid_argument &) {
                editText->setText(stringTools::numberToString(currentNumber));
            } catch (std::runtime_error &) {
                editText->setText(stringTools::numberToString(currentNumber));
            }
        });
        editText->registerEnterCallback([this, &settings]() {
            try {
                editText->setText(std::format("{}", currentNumber));
            } catch (std::runtime_error &) {
                editText->setText(stringTools::numberToString(currentNumber));
            }
        });
    }

    void registerEnterCallback(std::function<void()> callback) { editText->registerEnterCallback(callback); }
    void registerUpdatedCallback(std::function<void(T number)> callback) { updateCallbacks.push_back(callback); }

    void setNumber(const T &number) { editText->setText(stringTools::numberToString(number)); }
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
