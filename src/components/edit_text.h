#pragma once

#include "color.h"
#include "layout/scene.h"
#include "messure.h"
#include "renderables/text.h"
#include "text/typesetter.h"
#include "window.h"

namespace gltk {

struct EditTextSettings {
    std::string text = "Edit Text";
    std::optional<StyleSheet> styleSheet = std::nullopt;
    MessureVec2 size = MessureVec2(AbsoluteMessure(200), AbsoluteMessure(75));
    Style textStyle = Style({.color = Color(1.0f, 1.0f, 1.0f), .font = "Arial", .fontSize = 32});
    Style boxStyle = Style({.color = Color(0.1f, 0.1f, 0.1f), .radius = 5});
    Color selectColor = Color(0.3f, 0.3f, 0.8f);
    bool multiline = false;
    HorizontalTextAlign horizontalAlign = HorizontalTextAlign::Left;
    VerticalTextAlign verticalAlign = VerticalTextAlign::Center;
    float scrollTriggerSize = 20.0f;
    float scrollSpeed = 10.0f;
};

class EditText {
    std::vector<std::function<void(std::string)>> changeCallbacks;
    std::vector<std::function<void()>> enterCallbacks;
    std::vector<std::function<void()>> leaveCallbacks;

    EditTextSettings settings;

    Layout *box;
    Layout *text;

    bool dragging = false;
    TextAmount textAmount = TextAmount::Character;

    void notifyChange(const std::string &text);

  public:
    EditText(Scene *scene, Window *window, Layout *parent, EditTextSettings settings = {});

    void registerChangeCallback(std::function<void(std::string)> callback) { changeCallbacks.push_back(callback); }
    void registerEnterCallback(std::function<void()> callback) { enterCallbacks.push_back(callback); }
    void registerLeaveCallback(std::function<void()> callback) { leaveCallbacks.push_back(callback); }

    void setText(const std::string &text);
    std::string getText();
};

} // namespace gltk
