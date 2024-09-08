#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;
    window.add_key_down_callback([](Key key, KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });

    auto bg = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.1f, 0.1f, 0.1f), 0.0))
        .setSize(MessureVec2(1., 1.))
        .build();

    auto boxLayout = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 1.0f, 1.0f), 50.0))
        .setOffset(MessureVec2(200, 200))
        .setSize(MessureVec2(.5, .5))
        .setChildPlacement(ChildPlacement::List)
        .setListDirection(ListDirection::Down)
        .setOverflow(Overflow::Scroll)
        .build();
    
    auto child1 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 0.0f, 0.0f), 10.0))
        .setAnchor(Anchors::TopLeft)
        .setSize(MessureVec2(.8, 0.5))
        .build();

    auto child1child = LayoutBuilder(child1.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 1.0f, 0.0f), 10.0))
        .setAnchor(Anchors::TopCenter)
        .setPivot(Anchors::Center)
        .setSize(MessureVec2(200, 200))
        .build();
    



    window.run([&](Vec2 viewport) {
    });
    return 0;
}
