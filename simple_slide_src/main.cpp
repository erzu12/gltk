#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;

    auto background = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<Box>(Vec3(0.1f, 0.1f, 0.1f), 0.0))
        .setSize(MessureVec2(1., 1.))
        .setChildPlacement(ChildPlacement::ListStretch)
        .build();

    auto header = LayoutBuilder(background.get())
        .setRenderable(std::make_unique<Box>(Vec3(0.15f), 0.0))
        .setSize(MessureVec2(1., 100))
        .setChildPlacement(ChildPlacement::List)
        .setListDirection(ListDirection::Right)
        .build();

    auto boxButtonContainer = LayoutBuilder(header.get())
        .setSize(MessureVec2(150, 100))
        .build();

    auto boxButton = LayoutBuilder(boxButtonContainer.get())
        .setRenderable(std::make_unique<Box>(Vec3(0.1f, 0.1f, 0.1f), 10.0))
        .setSize(MessureVec2(.7, .6))
        .setAnchor(Anchors::Center)
        .build();

    auto boxButtonText = LayoutBuilder(boxButton.get())
        .setRenderable(std::make_unique<Text>("Box", 35.0, "DejaVuSans", Vec3(0.8f)))
        .setAnchor(Anchors::Center)
        .setSizing(Sizing::Fit, Sizing::Fit)
        .build();

    auto textButtonContainer = LayoutBuilder(header.get())
        .setSize(MessureVec2(150, 100))
        .build();

    auto textButton = LayoutBuilder(textButtonContainer.get())
        .setRenderable(std::make_unique<Box>(Vec3(0.1f, 0.1f, 0.1f), 10.0))
        .setSize(MessureVec2(.7, .6))
        .setAnchor(Anchors::Center)
        .build();

    auto textButtonText = LayoutBuilder(textButton.get())
        .setRenderable(std::make_unique<Text>("Text", 35.0, "DejaVuSans", Vec3(0.8f)))
        .setAnchor(Anchors::Center)
        .setSizing(Sizing::Fit, Sizing::Fit)
        .build();


    auto workspace = LayoutBuilder(background.get())
        .setSize(MessureVec2(1., 1.))
        .setOverflow(Overflow::Scroll)
        .build();

    auto slideBG = LayoutBuilder(workspace.get())
        .setRenderable(std::make_unique<Box>(Vec3(0.1f, 0.1f, 0.1f), 0.0))
        .setSize(MessureVec2(3., 3.))
        .setAnchor(Anchors::Center)
        .build();

    auto slide = LayoutBuilder(slideBG.get())
        .setRenderable(std::make_unique<Box>(Vec3(1., 1., 1.), 0.0))
        .setSize(MessureVec2(1920, 1080))
        .setAnchor(Anchors::Center)
        .build();

    bool dragging = false;
    Vec2 lastMousePos;
    workspace->addOnMouseKeyDownCallback([&](MouseButton button, KeyModifierFlags mods) {
        if (button == MouseButton::MOUSE_BUTTON_LEFT && mods.isModSet(KeyModifiers::MOD_CONTROL) || button == MouseButton::MOUSE_BUTTON_MIDDLE) {
            dragging = true;
        }
    });
    window.add_mouse_up_callback([&](MouseButton button, KeyModifierFlags mods) {
        dragging = false;
    });
    window.add_mouse_move_callback([&](Vec2 mousePos) {
        if (dragging) {
            Vec2 offset = mousePos - lastMousePos;
            workspace->scroll(offset);
        }
        lastMousePos = mousePos;
    });


    window.run([&](Vec2 viewport) {
    });
    return 0;
}
