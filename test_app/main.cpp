#include <iostream>
#include <gltk.h>

int main () {
    gltk::Window window;
    window.add_key_down_callback([](gltk::Key key, gltk::KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });

    auto boxLayout = LayoutBuilder()
        .setOffset(MessureVec2(0.0, 100))
        .setSize(MessureVec2(.5, .5))
        .setChildPlacement(ChildPlacement::ListStretch)
        .build();

    auto child1Layout = LayoutBuilder()
        .setOffset(MessureVec2(0.0, 0.0))
        .setSize(MessureVec2(.2, 1.0))
        .build();

    auto child2Layout = LayoutBuilder()
        .setOffset(MessureVec2(.0, 0.0))
        .setSize(MessureVec2(.4, 1.0))
        .build();

    auto child3Layout = LayoutBuilder()
        .setOffset(MessureVec2(.0, 0.0))
        .setSize(MessureVec2(200, 1.0))
        .build();


    gltk::Node node(std::move(boxLayout), std::make_unique<gltk::Box>(Vec3(0.0f, 0.0f, 1.0f), 100.0));
    window.get_node()->addChild(node);


    gltk::Node child1Node(std::move(child1Layout), std::make_unique<gltk::Box>(Vec3(1.0f, 0.0f, 0.0f), 100.0));
    gltk::Node child2Node(std::move(child2Layout), std::make_unique<gltk::Box>(Vec3(0.0f, 1.0f, 0.0f), 100.0));
    gltk::Node child3Node(std::move(child3Layout), std::make_unique<gltk::Box>(Vec3(1.0f, 1.0f, 0.0f), 50.0));
    node.addChild(child1Node);
    node.addChild(child2Node);
    node.addChild(child3Node);
    window.run([&](Vec2 viewport) {
    });
    return 0;
}
