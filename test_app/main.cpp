#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;
    window.add_key_down_callback([](gltk::Key key, gltk::KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });


    auto boxLayout = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 0.0f, 1.0f), 10.0))
        .setOffset(MessureVec2(10, 10))
        .setSize(MessureVec2(.5, .5))
        .setChildPlacement(ChildPlacement::ListStretch)
        .build();

    auto child1Layout = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 0.0f, 0.0f), 100.0))
        .setOffset(MessureVec2(0.0, 0.0))
        .setSize(MessureVec2(.2, 1.0))
        .build();

    auto child2Layout = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 1.0f, 0.0f), 100.0))
        .setOffset(MessureVec2(.0, 0.0))
        .setSize(MessureVec2(.4, 1.0))
        .build();

    auto child3Layout = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 1.0f, 0.0f), 50.0))
        .setOffset(MessureVec2(.0, 0.0))
        .setSize(MessureVec2(200, 1.0))
        .build();

    window.run([&](Vec2 viewport) {
    });
    return 0;
}
