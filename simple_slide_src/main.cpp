#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;

    auto backgroundColor = Color(0.1f);

    auto background = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<Box>(Style{.color=backgroundColor}))
        .setSize(MessureVec2(1., 1.))
        .setChildPlacement(ChildPlacement::ListStretch)
        .build();

    auto header = LayoutBuilder(background.get())
        .setRenderable(std::make_unique<Box>(Style{Color(0.15f)}))
        .setSize(MessureVec2(1., 100))
        .setChildPlacement(ChildPlacement::List)
        .setListDirection(ListDirection::Right)
        .build();

    auto boxButtonContainer = LayoutBuilder(header.get())
        .setSize(MessureVec2(150, 100))
        .build();

    auto buttonStyle = Style{.color=Color(0.1), .radius=10.0};
    auto buttonTextStyle = Style{.color=Color(0.8), .font="DejaVuSans", .fontSize=35};

    auto boxButton = LayoutBuilder(boxButtonContainer.get())
        .setRenderable(std::make_unique<Box>(buttonStyle))
        .setSize(MessureVec2(.7, .6))
        .setAnchor(Anchors::Center)
        .build();

    auto boxButtonText = LayoutBuilder(boxButton.get())
        .setRenderable(std::make_unique<Text>("Box", buttonTextStyle))
        .setAnchor(Anchors::Center)
        .setSizing(Sizing::Fit, Sizing::Fit)
        .build();

    auto textButtonContainer = LayoutBuilder(header.get())
        .setSize(MessureVec2(150, 100))
        .build();

    auto textButton = LayoutBuilder(textButtonContainer.get())
        .setRenderable(std::make_unique<Box>(buttonStyle))
        .setSize(MessureVec2(.7, .6))
        .setAnchor(Anchors::Center)
        .build();

    auto textButtonText = LayoutBuilder(textButton.get())
        .setRenderable(std::make_unique<Text>("Text", buttonTextStyle))
        .setAnchor(Anchors::Center)
        .setSizing(Sizing::Fit, Sizing::Fit)
        .build();


    auto workspace = LayoutBuilder(background.get())
        .setSize(MessureVec2(1., 1.))
        .setOverflow(Overflow::Scroll)
        .build();

    auto slideBG = LayoutBuilder(workspace.get())
        .setRenderable(std::make_unique<Box>(Style{.color=backgroundColor}))
        .setSize(MessureVec2(3., 3.))
        .setAnchor(Anchors::Center)
        .build();

    auto slide = LayoutBuilder(slideBG.get())
        .setRenderable(std::make_unique<Box>(Style{.color=Color::white()}))
        .setSize(MessureVec2(1920, 1080))
        .setAnchor(Anchors::Center)
        .build();

    bool dragging = false;
    Vec2 lastMousePos;
    workspace->addOnMouseKeyDownCallback([&](MouseButtonEvent e) {
        if (e.button == MouseButton::MOUSE_BUTTON_LEFT && e.mods.isModSet(KeyModifiers::MOD_CONTROL) || e.button == MouseButton::MOUSE_BUTTON_MIDDLE) {
            dragging = true;
        }
    });
    window.add_mouse_up_callback([&](auto _) {
        dragging = false;
    });
    window.add_mouse_move_callback([&](MouseMoveEvent e) {
        if (dragging) {
            Vec2 offset = e.pos - lastMousePos;
            workspace->scroll(offset);
        }
        lastMousePos = e.pos;
    });


    window.run([&](Vec2 viewport) {
    });
    return 0;
}
