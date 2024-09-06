#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;
    window.add_key_down_callback([](Key key, KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });

    auto boxLayout = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 1.0f, 1.0f), 10.0))
        .setOffset(MessureVec2(200, 200))
        .setSize(MessureVec2(.5, .5))
        .setChildPlacement(ChildPlacement::List)
        .setListDirection(ListDirection::Down)
        .setOverflow(Overflow::Scroll)
        .build();
    
    auto child1 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 0.0f, 0.0f), 10.0))
        .setAnchor(Anchors::TopCenter)
        .setSize(MessureVec2(.8, .5))
        .build();
    
    auto child2 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 1.0f, 0.0f), 10.0))
        .setAnchor(Anchors::TopCenter)
        .setSize(MessureVec2(.8, .5))
        .build();

    auto child3 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 0.0f, 1.0f), 10.0))
        .setAnchor(Anchors::TopCenter)
        .setSize(MessureVec2(.8, .5))
        .build();

    bool mouseDown = false;
    Vec2 lastMousePos;
    boxLayout->addOnMouseKeyDownCallback([&](MouseButton button, KeyModifierFlags mods) {
        mouseDown = true;
        lastMousePos = window.get_mouse_pos();
    });
    window.add_mouse_up_callback([&](MouseButton button, KeyModifierFlags mods) {
        mouseDown = false;
    });
    window.add_mouse_move_callback([&](Vec2 mousePos) {
        std::cout << "Mouse move: " << mousePos.x << ", " << mousePos.y << std::endl;
        if (mouseDown) {
            Vec2 offset = (mousePos - lastMousePos);
            boxLayout->scroll(offset);
        }
        lastMousePos = mousePos;
    });


    boxLayout->addOnScroleCallback([&](Vec2 scrole) {
        boxLayout->scroll(scrole * Vec2(30.0));
    });


    window.run([&](Vec2 viewport) {
    });
    return 0;
}
