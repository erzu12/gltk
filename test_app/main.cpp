#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;
    window.add_key_down_callback([](Key key, KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });

    auto bg = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<gltk::Box>(Style{Vec3(0.1f, 0.1f, 0.1f)}))
        .setSize(MessureVec2(1., 1.))
        .build();

    auto boxLayout = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<gltk::Box>(Style{.color = Vec3(1.0f, 1.0f, 1.0f), .radius = 50.0}))
        .setOffset(MessureVec2(200, 400))
        .setSize(MessureVec2(.6, .3))
        .setOverflow(Overflow::None)
        .setSizing(Sizing::Fixed, Sizing::Fixed)
        .build();
    
    auto child1 = std::make_unique<Layout>(boxLayout.get(), Positioning {
        .size = MessureVec2(1., 0.8),
        .anchor = Anchors::Center,
        .verticalSizing = Sizing::Expand,
        .horizontalSizing = Sizing::Expand}
        ,std::make_unique<gltk::Box>(Style{.color = Vec3(1.0f, 0.0f, 0.0f), .radius = 1.0, .rotation = PI / 4.0}));



    window.run([&](Vec2 viewport) {
    });
    return 0;
}
