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
    
    std::vector<Vec2> points = {
        Vec2(200, 100),
        Vec2(250, 100),
        Vec2(300, 150),
        Vec2(300, 200),

        Vec2(300, 200),
        Vec2(300, 250),
        Vec2(250, 300),
        Vec2(200, 300),

        Vec2(200, 300),
        Vec2(150, 300),
        Vec2(100, 250),
        Vec2(100, 200),

        Vec2(100, 200),
        Vec2(100, 150),
        Vec2(150, 100),
        Vec2(200, 100)
    };

    auto path = std::make_unique<PathObject>(points, Style{
            .color = Color::turquoise(), 
            .borderWidth = 40,
            .borderColor = Color::blue(),
        }, true, true);

    canvas->addObject(std::move(path));

    auto canvasLayout = LayoutBuilder(window.get_layout())
        .setRenderable(std::move(canvas))
        .setSize(MessureVec2(500, 500))
        .setOffset(MessureVec2(100, 100))
        .build();



    window.run([&](Vec2 viewport) {
    });
    return 0;
}
