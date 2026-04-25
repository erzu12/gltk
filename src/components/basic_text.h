#pragma once

#include "renderables/text.h"
#include <color.h>
#include <layout/scene.h>
#include <messure.h>

namespace gltk {

struct BasicTextSettings {
    std::string text = "Text";
    std::shared_ptr<StyleSheet> styleSheet = nullptr;
    Style textStyle = Style({.color = Color(1.0f, 1.0f, 1.0f), .font = "Arial", .fontSize = 32});
    HorizontalTextAlign horizontalAlign = HorizontalTextAlign::Left;
    VerticalTextAlign verticalAlign = VerticalTextAlign::Center;
};

class BasicText {
  public:
    BasicText(Scene *scene, Layout *parent, BasicTextSettings settings = {});
};

} // namespace gltk
