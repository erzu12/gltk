#pragma once

#include "color.h"
#include "layout/relative_scene.h"
#include <functional>

namespace gltk {

struct ToggleButtonSettings {
    int size = 50;
    float animationDuration = 0.15f;
    Color offColor = Color(0.3f, 0.3f, 0.3f);
    Color onColor = Color(0.3f, 0.3f, 1.0f);
    Color dotColor = Color(1.0f, 1.0f, 1.0f);
    std::function<float(float)> easingFunc = easing::easeInOutCirc;
};

class ToggleButton {
    bool isOn = false;
    std::vector<std::function<void(bool)>> stateChangeCallbacks;

  public:
    ToggleButton(RelativeScene *scene, RelativeLayout *parent, ToggleButtonSettings settings = {});

    bool getState() const { return isOn; }
    void registerStateChangeCallback(std::function<void(bool)> callback) { stateChangeCallbacks.push_back(callback); }
};

} // namespace gltk
