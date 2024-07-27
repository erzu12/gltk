#include <iostream>
#include <gltk.h>

int main () {
    gltk::Window window;
    window.add_key_down_callback([](gltk::Key key, gltk::KeyModifierFlags mods) {
        std::cout << "Key down: " << static_cast<int>(key) << std::endl;
    });
    gltk::Box box(Layout(Anchors::TopCenter, MessureVec2(0.0, 0.0), Anchors::TopCenter, MessureVec2(.5, .5)), 200.0);
    window.run([&](Vec2 viewport) {
        box.draw(viewport);
    });
    return 0;
}
