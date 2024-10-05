#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;
    window.add_key_down_callback([](auto e) {
        std::cout << "Key down: " << static_cast<int>(e.key) << std::endl;
    });

    auto bg = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<Box>(Style{Color(0.1f, 0.1f, 0.1f)}))
        .setSize(MessureVec2(1., 1.))
        .build();

    auto box = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<Box>(Style{.color = Color::white(), .radius = 0}))
        .setSize(MessureVec2(500, 500))
        .setOffset(MessureVec2(100, 100))
        .setChildPlacement(ChildPlacement::List)
        .build();

    auto child1 = LayoutBuilder(box.get())
        .setRenderable(std::make_unique<Box>(Style{.color = Color::red(), .radius = 0}))
        .setSize(MessureVec2(1.f, 100))
        .setAnchor(Anchors::Center)
        .setMargin(Margin{10, 10, 10, 10})
        .build();

    auto child2 = LayoutBuilder(box.get())
        .setRenderable(std::make_unique<Box>(Style{.color = Color::green(), .radius = 0}))
        .setSize(MessureVec2(1.f, 100))
        .setAnchor(Anchors::Center)
        .setMargin(Margin{10, 10, 10, 10})
        .build();

    auto child3 = LayoutBuilder(box.get())
        .setRenderable(std::make_unique<Box>(Style{.color = Color::blue(), .radius = 0}))
        .setSize(MessureVec2(1.f, 100))
        .setAnchor(Anchors::Center)
        .setMargin(Margin{10, 10, 10, 10})
        .build();




    window.run([&](Vec2 viewport) {
    });
    return 0;
}
