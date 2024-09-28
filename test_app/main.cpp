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

    auto canvas = std::make_unique<Canvas>(Style{.color = Color::snow()}, Vec2(500, 500));
    auto circle = std::make_unique<Oval>(Vec2(200, 200), Vec2(200, 200), Style{.color = Color::crimson()});
    auto rect = std::make_unique<Rectangle>(Vec2(400, 400), Vec2(100, 100), Style{.color = Color::green()});
    


    canvas->addObject(std::move(circle));
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
