#include "components/drop_down.h"
#include "components/edit_text.h"
#include "components/numeric_input.h"
#include "components/slider.h"
#include "styleSheet.h"
#include <components.h>
#include <gltk.h>
#include <iostream>

using namespace gltk;

int main() {
    gltk::Window window;
    window.add_key_down_callback([](auto e) { std::cout << "Key down: " << static_cast<int>(e.key) << std::endl; });
    auto rootLayout = window.getScene()->getRoot();

    auto styleSheet = getStyleSheet();

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

    // window.getScene()->addEventCallback<MouseMoveEvent>(
    //     [&](MouseMoveEvent &event) { std::cout << "Mouse moved at: " << event.pos << std::endl; }, svg.get()
    // );

    window.run([&](Vec2 viewport) {});
    return 0;
}
