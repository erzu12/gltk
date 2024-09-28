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
    auto circle = std::make_unique<Oval>(Vec2(150, 150), Vec2(200, 100), Style{.color = Color::crimson()});
    circle->rotate(PI / 4);
    circle->translate(Vec2(100, 100));
    circle->scale(Vec2(3));
    circle->rotate(PI / 2);
    auto rect = std::make_unique<Rectangle>(Vec2(400, 400), Vec2(100, 100), Style{.color = Color::green()});
    rect->rotate(PI / 4);
    


    canvas->addObject(std::move(circle));
    canvas->addObject(std::move(rect));

    auto canvasLayout = LayoutBuilder(window.get_layout())
        .setRenderable(std::move(canvas))
        .setSize(MessureVec2(500, 500))
        .setOffset(MessureVec2(100, 100))
        .build();

    auto box = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<Box>(Style{.color = Color::white(), .radius = 20}))
        .setSize(MessureVec2(600, 200))
        .setOffset(MessureVec2(100, 700))
        .build();

    auto text = LayoutBuilder(box.get())
        .setRenderable(std::make_unique<Text>("Hello World", Style{.color = Color::black(), .fontSize = 100}))
        .setSizing(Sizing::Fit, Sizing::Fit)
        .setAnchor(Anchors::Center)
        .build();



    window.run([&](Vec2 viewport) {
    });
    return 0;
}
