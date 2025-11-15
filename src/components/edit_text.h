#pragma once

#include "color.h"
#include "layout/scene.h"
#include "messure.h"
#include "renderables/text.h"
#include "window.h"

namespace gltk {

struct EditTextSettings {
    std::string text = "Edit Text";
    std::optional<StyleSheet> styleSheet = std::nullopt;
    MessureVec2 size = MessureVec2(AbsoluteMessure(200), AbsoluteMessure(75));
    Style textStyle = Style({.color = Color(1.0f, 1.0f, 1.0f), .font = "Arial", .fontSize = 32});
    Style boxStyle = Style({.color = Color(0.1f, 0.1f, 0.1f), .radius = 5});
    Color selectColor = Color(0.3f, 0.3f, 0.8f);
};

class EditText {
    std::vector<std::function<void()>> clickCallbacks;
    bool focused = false;
    bool dragging = false;
    TextAmount textAmount = TextAmount::Character;
    Vec2 dragStart;

  public:
    EditText(Scene *scene, Window *window, Layout *parent, EditTextSettings settings = {});
};

} // namespace gltk
