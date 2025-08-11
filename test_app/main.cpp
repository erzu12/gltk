#include "animation.h"
#include <gltk.h>
#include <iostream>

using namespace gltk;

class ToggleButton {
    bool isOn = false;

  public:
    ToggleButton(RelativeScene *scene, RelativeLayout *parent) {
        Color offColor = Color(0.3f, 0.3f, 0.3f);
        Color onColor = Color(0.3f, 0.3f, 1.0f);

        auto toggleRoot = LayoutBuilder(scene).setSize(MessureVec2(100_px, 50_px)).build();
        auto toggleSlider = LayoutBuilder(scene, toggleRoot)
                                .setRenderable(std::make_unique<Box>(Style{.color = offColor, .radius = 50}))
                                .setSize(MessureVec2(100_pct, 100_pct))
                                .build();
        auto toggleDot =
            LayoutBuilder(scene, toggleRoot)
                .setRenderable(std::make_unique<Box>(Style{.color = Color(1.0f, 1.0f, 1.0f), .radius = 50}))
                .setAnchor(Anchors::CenterLeft)
                .setSize(MessureVec2(40_px, 40_px))
                .setOffset(MessureVec2(25_px, 0_px))
                .build();

        scene->addEventCallback<MouseButtonEvent>(
            [=, this](MouseButtonEvent &event) {
                if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::PRESS) {
                    Style *style = toggleSlider->renderable.value()->getStyle();
                    if (isOn) {
                        scene->addAnimation(toggleDot->positioning.offset.x.get(), 75_px, 25_px, 0.1f);
                        scene->addAnimation(&style->color, onColor, offColor, 0.1f);
                    } else {
                        scene->addAnimation(toggleDot->positioning.offset.x.get(), 25_px, 75_px, 0.1f);
                        scene->addAnimation(&style->color, offColor, onColor, 0.1f);
                    }
                    isOn = !isOn;
                }
            },
            toggleRoot
        );
    }
};

int main() {
    gltk::Window window;
    window.add_key_down_callback([](auto e) { std::cout << "Key down: " << static_cast<int>(e.key) << std::endl; });
    auto rootLayout = window.getScene()->getRoot();

    auto bg = LayoutBuilder(window.getScene())
                  .setSize(MessureVec2(80_pct, 80_pct))
                  .setChildPlacement(ChildPlacement::ListStretch)
                  .build();

    ToggleButton toggleButton(window.getScene(), bg);

    // window.getScene()->addEventCallback<MouseMoveEvent>(
    //     [&](MouseMoveEvent &event) { std::cout << "Mouse moved at: " << event.pos << std::endl; }, svg.get()
    // );

    window.run([&](Vec2 viewport) {});
    return 0;
}
