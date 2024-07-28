#include <iostream>
#include <gltk.h>

int main () {
    gltk::Window window;
    window.add_key_down_callback([](gltk::Key key, gltk::KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });
    Layout boxLayout(window.get_layout(), Anchors::TopCenter, MessureVec2(0.0, 100), Anchors::TopCenter, MessureVec2(.5, .5));
    gltk::Box box(boxLayout, Vec3(0.0f, 0.0f, 1.0f), 100.0);
    Layout childBoxLayout(&boxLayout, Anchors::TopCenter, MessureVec2(0.0, 0.0), Anchors::TopCenter, MessureVec2(.2, .2));
    gltk::Box childBox(childBoxLayout, Vec3(1.0f, 0.0f, 0.0f), 10.0);
    window.run([&](Vec2 viewport) {
        box.draw(viewport);
        childBox.draw(viewport);
    });
    return 0;
}
