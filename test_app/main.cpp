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

    auto svg = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<SVGImage>("assets/fleet.svg", Style{.radius = 20}))
        .setSize(MessureVec2(0.5, 0.5))
        .setOffset(MessureVec2(100, 500))
        .setSizing(Sizing::Fixed, Sizing::Fit)
        .build();

    auto image = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<Image>("assets/image.png", Style{.radius = 20}))
        .setSize(MessureVec2(0.5, 0.5))
        .setOffset(MessureVec2(100, 1200))
        .setSizing(Sizing::Fixed, Sizing::Fit)
        .build();




    window.run([&](Vec2 viewport) {
    });
    return 0;
}
