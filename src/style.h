#pragma once
#include "color.h"
#include "fontloader.h"
#include <unordered_map>

namespace gltk {

struct Style {
    Color color = Color(1.0);
    std::string font = "Arial";
    FontSlant slant = FontSlant::Roman;
    FontWeight weight = FontWeight::Normal;
    int fontSize = 16;
    float radius = 0.0f;
    float borderWidth = 0.0f;
    Color borderColor = Color();
    float rotation = 0.0f;
};

class StyleSheet {
  private:
    std::unordered_map<std::string, Style> styles;

  public:
    StyleSheet(std::unordered_map<std::string, Style> styles) : styles(std::move(styles)) {}

    void setStyle(const std::string &name, const Style &style);

    Style getStyle(const std::string &name) const;
};

} // namespace gltk
