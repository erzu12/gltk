#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;
    window.add_key_down_callback([](Key key, KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });

    auto bg = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<Box>(Style{Color(0.1f, 0.1f, 0.1f)}))
        .setSize(MessureVec2(1., 1.))
        .build();

    auto canvas = std::make_unique<Canvas>(Style{.color = Color(0.9f, 0.9f, 0.9f)}, Vec2(500, 500));
    auto rect = std::make_unique<Rectangle>(Vec2(100, 200), Vec2(100, 200), Style{.color = Color::red()});
    


    canvas->addObject(std::move(rect));

    auto canvasLayout = LayoutBuilder(window.get_layout())
        .setRenderable(std::move(canvas))
        .setSize(MessureVec2(500, 500))
        .setOffset(MessureVec2(100, 100))
        .build();



    window.run([&](Vec2 viewport) {
    });
    return 0;
}
