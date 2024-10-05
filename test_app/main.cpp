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
        .setSize(MessureVec2(400, 400))
        .setOffset(MessureVec2(100, 100))
        .build();

    auto child1 = LayoutBuilder(box.get())
        .setRenderable(std::make_unique<Box>(Style{.color = Color::red(), .radius = 0}))
        .setSize(MessureVec2(200, 200))
        .setAnchor(Anchors::Center)
        .setSizing(Sizing::Expand, Sizing::Expand)
        .setMargin({50, 50, 50, 50})
        .setPadding({100, 0, 0, 100})
        .build();

    auto child2 = LayoutBuilder(child1.get())
        .setRenderable(std::make_unique<Box>(Style{.color = Color(.0f, 1.f, .0f, .5f), .radius = 0}))
        .setSize(MessureVec2(200, 200))
        .setAnchor(Anchors::Center)
        .build();





    window.run([&](Vec2 viewport) {
    });
    return 0;
}
