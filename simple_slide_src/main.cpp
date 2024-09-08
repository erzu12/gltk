#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;

    auto background = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.1f, 0.1f, 0.1f), 0.0))
        .setSize(MessureVec2(1., 1.))
        .setChildPlacement(ChildPlacement::ListStretch)
        .build();

    auto header = LayoutBuilder(background.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.2f, 0.2f, 0.2f), 0.0))
        .setSize(MessureVec2(1., 150))
        .build();

    auto workspace = LayoutBuilder(background.get())
        .setSize(MessureVec2(1., 1.))
        .setOverflow(Overflow::Scroll)
        .build();

    auto slideBG = LayoutBuilder(workspace.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.1f, 0.1f, 0.1f), 0.0))
        .setSize(MessureVec2(3., 3.))
        .setAnchor(Anchors::Center)
        .build();

    auto slide = LayoutBuilder(slideBG.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1., 1., 1.), 0.0))
        .setSize(MessureVec2(1920, 1080))
        .setAnchor(Anchors::Center)
        .build();

    bool dragging = false;
    Vec2 lastMousePos;
    workspace->addOnMouseKeyDownCallback([&](MouseButton button, KeyModifierFlags mods) {
        if (button == MouseButton::MOUSE_BUTTON_LEFT && mods.isModSet(KeyModifiers::MOD_CONTROL) || button == MouseButton::MOUSE_BUTTON_MIDDLE) {
            dragging = true;
            lastMousePos = window.get_mouse_pos();
        }
    });
    window.add_mouse_up_callback([&](MouseButton button, KeyModifierFlags mods) {
        dragging = false;
    });
    window.add_mouse_move_callback([&](Vec2 mousePos) {
        if (dragging) {
            Vec2 offset = (mousePos - lastMousePos);
            workspace->scroll(offset);
        }
        lastMousePos = mousePos;
    });


    window.run([&](Vec2 viewport) {
    });
    return 0;
}
