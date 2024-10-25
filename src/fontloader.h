#pragma once

#include <fontconfig/fontconfig.h>
#include <iostream>

enum class FontSlant {
    Roman = FC_SLANT_ROMAN,
    Italic = FC_SLANT_ITALIC,
    Oblique = FC_SLANT_OBLIQUE
};

enum class FontWeight {
    Thin = FC_WEIGHT_THIN,
    ExtraLight = FC_WEIGHT_EXTRALIGHT,
    Light = FC_WEIGHT_LIGHT,
    Normal = FC_WEIGHT_NORMAL,
    Medium = FC_WEIGHT_MEDIUM,
    SemiBold = FC_WEIGHT_SEMIBOLD,
    Bold = FC_WEIGHT_BOLD,
    ExtraBold = FC_WEIGHT_EXTRABOLD,
    Black = FC_WEIGHT_BLACK
};

class FontLoader {
    FcConfig* config;
public:
    FontLoader();
    ~FontLoader();

    std::string getFontPath(std::string fontName, FontSlant slant = FontSlant::Roman, FontWeight weight = FontWeight::Normal);
};
