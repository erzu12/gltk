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
        .setRenderable(std::make_unique<Box>(Style{.color = Color::white(), .radius = 20}))
        .setSize(MessureVec2(0.5, 300))
        .setOffset(MessureVec2(100, 100))
        .build();

    auto canvas = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<Canvas>(Style{}, Vec2(300, 300)))
        .setSize(MessureVec2(300, 300))
        .setOffset(MessureVec2(100, 450))
        .build();

    auto image = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<Image>("assets/image.png", Style{.radius = 20}))
        .setSize(MessureVec2(0.5, 0.5))
        .setOffset(MessureVec2(100, 800))
        .setSizing(Sizing::Fixed, Sizing::Fit)
        .build();




    window.run([&](Vec2 viewport) {
    });
    return 0;
}
