#pragma once

#include "color.h"
#include "layout/scene.h"
#include "messure.h"

namespace gltk {

struct ButtonSettings {
    std::string text = "Button";
    std::optional<StyleSheet> styleSheet = std::nullopt;
    MessureVec2 size = MessureVec2(AbsoluteMessure(200), AbsoluteMessure(75));
    Style boxStyle = Style({.color = Color(0.3f, 0.3f, 0.3f), .radius = 10.0f});
    Style textStyle = Style({.color = Color(1.0f, 1.0f, 1.0f), .font = "Arial", .fontSize = 32});
    Color activeColor = Color(0.2f, 0.2f, 0.2f);
    Color hoverColor = Color(0.4f, 0.4f, 0.4f);
};

class Button {
    std::vector<std::function<void()>> clickCallbacks;

  public:
    Button(Scene *scene, Layout *parent, ButtonSettings settings = {});

    void registerClickCallback(std::function<void()> callback) { clickCallbacks.push_back(callback); }
};

} // namespace gltk
