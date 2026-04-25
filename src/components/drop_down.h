#pragma once

#include <color.h>
#include <layout/scene.h>
#include <messure.h>

namespace gltk {

struct DropDownSettings {
    std::string initialText = "Button";
    std::vector<std::string> options = {};
    std::shared_ptr<StyleSheet> styleSheet = nullptr;
    MessureVec2 size = MessureVec2(AbsoluteMessure(300), AbsoluteMessure(75));
    Style boxStyle = Style({.color = Color(0.3f, 0.3f, 0.3f), .radius = 10.0f});
    Style textStyle = Style({.color = Color(1.0f, 1.0f, 1.0f), .font = "Arial", .fontSize = 32});
    Color activeColor = Color(0.2f, 0.2f, 0.2f);
    Color hoverColor = Color(0.4f, 0.4f, 0.4f);
};

class DropDown {
    std::vector<std::function<void(std::string)>> changeCallbacks;
    DropDownSettings settings;

  public:
    DropDown(Scene *scene, Layout *parent, DropDownSettings settings = {});

    void registerChangeCallback(std::function<void(std::string)> callback) { changeCallbacks.push_back(callback); }
};

} // namespace gltk
