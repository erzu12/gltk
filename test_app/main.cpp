#include <components.h>
#include <gltk.h>
#include <iostream>

using namespace gltk;

int main() {
    gltk::Window window;
    window.add_key_down_callback([](auto e) { std::cout << "Key down: " << static_cast<int>(e.key) << std::endl; });
    auto rootLayout = window.getScene()->getRoot();

    auto bg = LayoutBuilder(window.getScene())
                  .setSize(MessureVec2(80_pct, 80_pct))
                  .setChildPlacement(ChildPlacement::ListStretch)
                  .build();

    ToggleButton toggleButton(window.getScene(), bg, {.animationDuration = 0.5});
    toggleButton.registerStateChangeCallback([](bool state) {
        std::cout << "Toggle state changed: " << (state ? "ON" : "OFF") << std::endl;
    });

    Button button(window.getScene(), bg);

    // window.getScene()->addEventCallback<MouseMoveEvent>(
    //     [&](MouseMoveEvent &event) { std::cout << "Mouse moved at: " << event.pos << std::endl; }, svg.get()
    // );

    window.run([&](Vec2 viewport) {});
    return 0;
}
