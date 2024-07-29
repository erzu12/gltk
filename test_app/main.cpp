#include <iostream>
#include <gltk.h>

int main () {
    gltk::Window window;
    window.add_key_down_callback([](gltk::Key key, gltk::KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });

    auto boxLayout = LayoutBuilder()
        .setOffset(MessureVec2(0.0, 100))
        .setSize(MessureVec2(.5, .5))
        .setChildPlacement(ChildPlacement::ListStretch)
        .build();

    auto child1Layout = LayoutBuilder()
        .setOffset(MessureVec2(0.0, 0.0))
        .setSize(MessureVec2(.2, 1.0))
        .build();

    auto child2Layout = LayoutBuilder()
        .setOffset(MessureVec2(.0, 0.0))
        .setSize(MessureVec2(.4, 1.0))
        .build();

    auto child3Layout = LayoutBuilder()
        .setOffset(MessureVec2(.0, 0.0))
        .setSize(MessureVec2(200, 1.0))
        .build();


    window.get_layout()->addChild(boxLayout.get());

    gltk::Box box(boxLayout, Vec3(0.0f, 0.0f, 1.0f), 100.0);
    gltk::Box child1Box(child1Layout, Vec3(1.0f, 0.0f, 0.0f), 100.0);
    gltk::Box child2Box(child2Layout, Vec3(0.0f, 1.0f, 0.0f), 100.0);
    gltk::Box child3Box(child3Layout, Vec3(1.0f, 1.0f, 0.0f), 50.0);
    box.addChild(child1Box);
    box.addChild(child2Box);
    box.addChild(child3Box);
    window.run([&](Vec2 viewport) {
        box.draw(viewport);
        child1Box.draw(viewport);
        child2Box.draw(viewport);
        child3Box.draw(viewport);
    });
    return 0;
}
