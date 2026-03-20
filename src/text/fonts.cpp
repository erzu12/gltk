#include "fonts.h"
#include <glad/glad.h>

namespace gltk {

Font::Font(const FontSettings &settings, FontLoader &fontLoader, FT_Library &ft) {
    loadCharacters(settings, fontLoader, ft);
}

void Font::loadCharacters(const FontSettings &settings, FontLoader &fontLoader, FT_Library &ft) {
    fontSize = settings.fontSize;
    std::string fontPath = fontLoader.getFontPath(settings.font, settings.slant, settings.weight);

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, settings.fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++) {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Glyph character = {
            texture,
            Vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters.insert(std::pair<char, Glyph>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

FontManager::FontManager() {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
}

Font *FontManager::getFont(FontSettings settings) {
    size_t hash = std::hash<std::string>()(settings.font) ^ std::hash<int>()(static_cast<int>(settings.slant)) ^
                  std::hash<int>()(static_cast<int>(settings.weight)) ^ std::hash<int>()(settings.fontSize);
    auto it = fonts.find(hash);
    if (it != fonts.end()) {
        return it->second.get();
    } else {
        auto font = std::make_unique<Font>(settings, fontLoader, ft);
        fonts.insert({hash, std::move(font)});
        return fonts[hash].get();
    }
}

} // namespace gltk
