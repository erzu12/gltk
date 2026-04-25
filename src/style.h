#pragma once
#include "animation.h"
#include "color.h"
#include "text/fonts.h"

namespace gltk {

struct Style {
    Animatable<Color> color = Color::transparent();
    std::string font = "Arial";
    FontSlant slant = FontSlant::Roman;
    FontWeight weight = FontWeight::Normal;
    Animatable<int> fontSize = 16;
    Animatable<float> radius = 0.0f;
    Animatable<float> borderWidth = 0.0f;
    Animatable<Color> borderColor = Color::transparent();
    Animatable<float> rotation = 0.0f;
};

struct StyleSheet {
    virtual Style primaryBackground() { return Style{.color = gltk::Color(0.15f)}; };
    virtual Style primaryForeground() { return Style{.color = gltk::Color(1.0f), .font = "Arial", .fontSize = 30}; };
    virtual Style accentBackground() { return Style{.color = gltk::Color(0.3f, 0.3f, 1.0f)}; };
    virtual Style accentForeground() { return Style{.color = gltk::Color(1.0f), .font = "Arial", .fontSize = 30}; };
    virtual Style inputBackground() { return Style{.color = gltk::Color(0.25f, 0.25f, 0.25f), .radius = 10.0f}; };
    virtual Style inputHover() { return Style{.color = gltk::Color(0.3f, 0.3f, 0.3f)}; };
    virtual Style inputActive() { return Style{.color = gltk::Color(0.2f, 0.2f, 0.2f)}; };
};

} // namespace gltk
