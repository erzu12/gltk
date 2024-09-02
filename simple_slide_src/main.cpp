#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;
    window.add_key_down_callback([](Key key, KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });

    auto background = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.1f, 0.1f, 0.1f), 0.0))
        .setSize(MessureVec2(1., 1.))
        .setChildPlacement(ChildPlacement::ListStretch)
        .build();

    auto header = LayoutBuilder(background.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.2f, 0.2f, 0.2f), 0.0))
        .setSize(MessureVec2(1., 150))
        .build();

    auto workspace = LayoutBuilder(background.get())
        .setSize(MessureVec2(1., 1.))
        .build();

    auto slideBG = LayoutBuilder(workspace.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1., 1., 1.), 0.0))
        .setSize(MessureVec2(1920, 1080))
        .setAnchor(Anchors::Center)
        .setPivot(Anchors::Center)
        .build();


    window.run([&](Vec2 viewport) {
    });
    return 0;
}
