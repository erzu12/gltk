#pragma once

#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include <memory>
#include <unordered_map>
#include <vec_math.h>
#include FT_FREETYPE_H

namespace gltk {

struct Glyph {
    unsigned int textureID;
    Vec2 size;
    Vec2 bearing;
    long advance;
};

enum class FontSlant { Roman = FC_SLANT_ROMAN, Italic = FC_SLANT_ITALIC, Oblique = FC_SLANT_OBLIQUE };

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

struct FontSettings {
    std::string font = "Arial";
    FontSlant slant = FontSlant::Roman;
    FontWeight weight = FontWeight::Normal;
    int fontSize = 16;
};

class Font {

  public:
    Font(const FontSettings &settings, FcConfig *fontConfig, FT_Library &ft);
    ~Font();

    Glyph getGlyph(unsigned long charCode);

    float heightCap = 0;
    float descent = 0;
    int fontSize = 0;

    FontSettings getSettings() { return settings; }

  private:
    FontSettings settings;
    FT_Face face;
    FT_Library ft;
    std::unordered_map<unsigned long, Glyph> characters;
    FcConfig *fontConfig = nullptr;
    FcPattern *fontPattern = nullptr;
    void loadCharacter(unsigned long charCode);
    std::string getFontPathForPattern(FcPattern *pattern);
    bool checkCharInFont(unsigned long charCode);
    std::string getFallbackFontPath(unsigned long charCode);
};

class FontManager {
  public:
    FontManager();

    Font *getFont(FontSettings settings);

  private:
    std::unique_ptr<FT_LibraryRec_, void (*)(FT_Library)> freeType = {nullptr, [](FT_Library library) {
                                                                          FT_Done_FreeType(library);
                                                                      }};
    std::unique_ptr<FcConfig, decltype(&FcConfigDestroy)> fontConfig = {nullptr, FcConfigDestroy};
    std::unordered_map<size_t, std::unique_ptr<Font>> fonts;
};

} // namespace gltk
