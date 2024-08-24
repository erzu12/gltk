#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;
    window.add_key_down_callback([](Key key, KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });

    auto boxLayout = LayoutBuilder(window.get_layout())
        //.setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 1.0f, 1.0f), 10.0))
        .setOffset(MessureVec2(10, 10))
        .setSize(MessureVec2(.5, .5))
        .build();

    auto button = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 0.0f, 0.6f), 10.0))
        .setSize(MessureVec2(300, 100))
        .setPivot(Anchors::Center)
        .setAnchor(Anchors::Center)
        .build();

    auto textLayout = LayoutBuilder(button.get())
        .setRenderable(std::make_unique<gltk::Text>("Click me", 48, "Arial", Vec3(1.0, 1.0, 1.0), HorizontalTextAlign::Left, VerticalTextAlign::Top))
        .setPivot(Anchors::Center)
        .setAnchor(Anchors::Center)
        .setSizing(Sizing::Fit, Sizing::Fit)
        .build();

    window.run([&](Vec2 viewport) {
    });
    return 0;
}
