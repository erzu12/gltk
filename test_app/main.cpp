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
        .setOverflow(Overflow::Clip)
        .build();

    auto child1 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 0.0f, 0.0f), 10.0))
        .setSize(MessureVec2(.8, .7))
        .setAnchor(Anchors::TopCenter)
        .build();

    auto child2 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 1.0f, 0.0f), 10.0))
        .setAnchor(Anchors::TopCenter)
        .setSize(MessureVec2(.8, .7))
        .build();

    auto child3 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 0.0f, 1.0f), 10.0))
        .setAnchor(Anchors::TopCenter)
        .setSize(MessureVec2(.8, .7))
        .build();


    window.run([&](Vec2 viewport) {
    });
    return 0;
}
