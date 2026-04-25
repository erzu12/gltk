#include <components.h>
#include <gltk.h>
#include <iostream>

using namespace gltk;

struct CustomStyle : public StyleSheet {};

int main() {
    gltk::Window window;
    window.add_key_down_callback([](auto e) { std::cout << "Key down: " << static_cast<int>(e.key) << std::endl; });
    auto rootLayout = window.getScene()->getRoot();

    auto styleSheet = std::make_shared<CustomStyle>();

    auto bg = LayoutBuilder(window.getScene())
                  .setSize(MessureVec2(80_pct, 80_pct))
                  .setChildPlacement(ChildPlacement::ListStretch)
                  .build();

    ToggleButton toggleButton(window.getScene(), bg, {.styleSheet = styleSheet});
    toggleButton.registerStateChangeCallback([](bool state) {
        std::cout << "Toggle state changed: " << (state ? "ON" : "OFF") << std::endl;
    });

    Button button(
        window.getScene(), bg, {.text = "H-ef+", .styleSheet = styleSheet, .size = MessureVec2(100_px, 50_px)}
    );
    EditText editText(window.getScene(), &window, bg, {.text = "Edit Me", .styleSheet = styleSheet});
    editText.registerChangeCallback([](const std::string &text) { std::cout << "Text changed: " << text << std::endl; }
    );
    NumericInput<double> numericInput(window.getScene(), &window, bg, {.number = 42, .styleSheet = styleSheet});
    numericInput.registerUpdatedCallback([](long number) { std::cout << "Number changed: " << number << std::endl; });
    Slider slider(
        window.getScene(),
        &window,
        bg,
        {.initialValue = 50, .minValue = 0, .maxValue = 100, .step = 0, .styleSheet = styleSheet}
    );
    slider.registerValueChangeCallback([](double value) { std::cout << "Slider value changed: " << value << std::endl; }
    );
    DropDown dropDown = DropDown(
        window.getScene(),
        bg,
        {.initialText = "Select Option", .options = {"Option 1", "Option 2", "Option 3"}, .styleSheet = styleSheet}
    );
    dropDown.registerChangeCallback([](const std::string &option) {
        std::cout << "Selected option: " << option << std::endl;
    });

    Layout *canvasLayout =
        LayoutBuilder(window.getScene(), bg)
            .setRenderable(std::make_unique<Canvas>(Style{.color = Color(0.2f, 0.2f, 0.8f)}, Vec2(1000, 1000)))
            .setSizing({SizingMode::Content, SizingMode::Content})
            .build();

    std::vector<Vec2> bezierPoints{
        Vec2(0, 0),
        Vec2(0, 200),
        Vec2(200, 200),
        Vec2(200, 400),
    };
    std::unique_ptr<PathObject> bezier = std::make_unique<PathObject>(
        bezierPoints, Vec2(50, 50), Style{.borderWidth = 5, .borderColor = Color(1.0f, 0.0f, 0.0f)}, true
    );
    canvasLayout->getRenderable<Canvas>()->addObject(std::move(bezier));

    window.run([&](Vec2 viewport) {});
    return 0;
}
