#include <iostream>
#include <gltk.h>

int main () {
    gltk::Window window;
    window.add_key_down_callback([](gltk::Key key, gltk::KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });

    auto boxLayout = LayoutBuilder()
        .setAnchor(Anchors::TopCenter)
        .setOffset(MessureVec2(0.0, 100))
        .setPivot(Anchors::TopCenter)
        .setSize(MessureVec2(.5, .5))
        .build();

    auto childBoxLayout = LayoutBuilder()
        .setAnchor(Anchors::BottomLeft)
        .setOffset(MessureVec2(0.0, 100))
        .setPivot(Anchors::TopLeft)
        .setSize(MessureVec2(.2, .2))
        .build();

    window.get_layout()->addChild(boxLayout.get());

    gltk::Box box(boxLayout, Vec3(0.0f, 0.0f, 1.0f), 100.0);
    gltk::Box childBox(childBoxLayout, Vec3(1.0f, 0.0f, 0.0f), 10.0);
    box.addChild(childBox);
    window.run([&](Vec2 viewport) {
        box.draw(viewport);
        childBox.draw(viewport);
    });
    return 0;
}
