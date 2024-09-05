#include <iostream>
#include <gltk.h>

using namespace gltk;

int main () {
    gltk::Window window;
    window.add_key_down_callback([](Key key, KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });

    auto boxLayout = LayoutBuilder(window.get_layout())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 1.0f, 1.0f), 10.0))
        .setOffset(MessureVec2(200, 200))
        .setSize(MessureVec2(.5, .5))
        .setChildPlacement(ChildPlacement::List)
        .setListDirection(ListDirection::Down)
        .setOverflow(Overflow::Scroll)
        .build();
    
    auto child1 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(1.0f, 0.0f, 0.0f), 10.0))
        .setAnchor(Anchors::TopCenter)
        .setSize(MessureVec2(.8, .5))
        .build();
    
    auto child2 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 1.0f, 0.0f), 10.0))
        .setAnchor(Anchors::TopCenter)
        .setSize(MessureVec2(.8, .5))
        .build();

    auto child3 = LayoutBuilder(boxLayout.get())
        .setRenderable(std::make_unique<gltk::Box>(Vec3(0.0f, 0.0f, 1.0f), 10.0))
        .setAnchor(Anchors::TopCenter)
        .setSize(MessureVec2(.8, .5))
        .build();


    boxLayout->addOnScroleCallback([&](Vec2 scrole) {
        boxLayout->scroll(scrole * Vec2(30.0));
        window.get_layout()->resolveTransform();
        boxLayout->registerForRenderRecursive(window.get_renderer());
    });


    window.run([&](Vec2 viewport) {
    });
    return 0;
}
