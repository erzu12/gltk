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
        .setOffset(MessureVec2(10, 10))
        .setSize(MessureVec2(.5, .5))
        .build();


    auto childLayout = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 0.0f, 0.0f), 10.0))
        .setOffset(MessureVec2(.5, .5))
        .setSize(MessureVec2(.5, .5))
        .build();

    auto dot = LayoutBuilder(childLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 1.0f, 0.0f), 10.0))
        .setSize(MessureVec2(10, 10))
        .setPivot(Anchors::Center)
        .setAnchor(Anchors::Center)
        .build();

    auto textLayout = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Text>(Vec3(0.0f, 0.0f, 0.0f), "Hello World"))
        .setOffset(MessureVec2(.5, .5))
        .setSize(MessureVec2(.5, .5))
        .build();


    window.run([&](Vec2 viewport) {
    });
    return 0;
}
