#pragma once

#include "fontloader.h"
#include "vec_math.h"
#include <ft2build.h>
#include <memory>
#include <unordered_map>
#include FT_FREETYPE_H

namespace gltk {

struct Glyph {
    unsigned int textureID;
    Vec2 size;
    Vec2 bearing;
    long advance;
};

struct FontSettings {
    std::string font = "Arial";
    FontSlant slant = FontSlant::Roman;
    FontWeight weight = FontWeight::Normal;
    int fontSize = 16;
};

class Font {
  public:
    Font(const FontSettings &settings, FontLoader &fontLoader, FT_Library &ft);

    std::unordered_map<char, Glyph> characters;
    int fontSize = 0;

  private:
    void loadCharacters(const FontSettings &settings, FontLoader &fontLoader, FT_Library &ft);
};

class FontManager {
  public:
    FontManager();

    Font *getFont(FontSettings settings);

  private:
    std::unordered_map<size_t, std::unique_ptr<Font>> fonts;
    FontLoader fontLoader;
    FT_Library ft;
};

} // namespace gltk
