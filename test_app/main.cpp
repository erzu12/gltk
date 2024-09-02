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
        .setChildPlacement(ChildPlacement::ListStretch)
        .setListDirection(ListDirection::Left)
        //.setOverflow(Overflow::Clip)
        .build();

    auto child0 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 0.0f, 1.0f), 10.0))
        .setSize(MessureVec2(200, .8))
        .setAnchor(Anchors::Center)
        .build();

    auto child1 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 0.0f, 0.0f), 10.0))
        .setSize(MessureVec2(.8, .8))
        .setAnchor(Anchors::Center)
        .setSizing(Sizing::Fit, Sizing::Fixed)
        .build();

    auto child1child1 = LayoutBuilder(child1.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 1.0f, 1.0f), 10.0))
        .setSize(MessureVec2(200, 200))
        .setAnchor(Anchors::Center)
        .build();


    auto child2 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 1.0f, 0.0f), 10.0))
        .setAnchor(Anchors::Center)
        .setSize(MessureVec2(.8, .8))
        .build();

    auto child3 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 0.0f, 1.0f), 10.0))
        .setAnchor(Anchors::Center)
        .setSize(MessureVec2(.8, .8))
        .setSizing(Sizing::Expand, Sizing::Expand)
        .build();


    auto child3child1 = LayoutBuilder(child3.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 1.0f, 0.0f), 10.0))
        .setSize(MessureVec2(300, 300))
        .setAnchor(Anchors::Center)
        .build();


    window.run([&](Vec2 viewport) {
    });
    return 0;
}
