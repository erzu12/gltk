#pragma once
#include <gltk.h>

gltk::StyleSheet getStyleSheet() {
    return gltk::StyleSheet(
        {{"primaryBackground", gltk::Style{.color = gltk::Color(0.15f)}},
         {"primaryForeground", gltk::Style{.color = gltk::Color(1.0f), .font = "Arial", .fontSize = 30}},
         {"accentBackground", gltk::Style{.color = gltk::Color(0.3f, 0.3f, 1.0f)}},
         {"accentForeground", gltk::Style{.color = gltk::Color(1.0f), .font = "Arial", .fontSize = 30}},
         {"inputBackground", gltk::Style{.color = gltk::Color(0.25f, 0.25f, 0.25f), .radius = 10.0f}},
         {"inputHover", gltk::Style{.color = gltk::Color(0.3f, 0.3f, 0.3f)}},
         {"inputActive", gltk::Style{.color = gltk::Color(0.2f, 0.2f, 0.2f)}}}
    );
}
