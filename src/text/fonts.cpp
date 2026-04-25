#include "fonts.h"
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
// clang-format on

namespace gltk {

Font::Font(const FontSettings &settings, FcConfig *fontConfig, FT_Library &ft) {
    this->settings = settings;
    this->fontConfig = fontConfig;
    this->ft = ft;
    FcPattern *pat = FcNameParse((const FcChar8 *)settings.font.c_str());
    FcPatternAddInteger(pat, FC_SLANT, static_cast<int>(settings.slant));
    FcPatternAddInteger(pat, FC_WEIGHT, static_cast<int>(settings.weight));
    if (!pat) {
        throw std::runtime_error("Failed to create font pattern");
    }

    // Perform font matching
    FcConfigSubstitute(fontConfig, pat, FcMatchPattern);
    FcDefaultSubstitute(pat);
    fontPattern = pat;

    fontSize = settings.fontSize;
    std::string fontPath = getFontPathForPattern(pat);

    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, settings.fontSize);
    this->descent = face->size->metrics.descender >> 6;
    this->heightCap = getGlyph('H').bearing.y;
}

Glyph Font::getGlyph(unsigned long c) {
    auto it = characters.find(c);
    if (it != characters.end()) {
        return it->second;
    } else {
        loadCharacter(c);
        return characters[c];
    }
}

std::string Font::getFontPathForPattern(FcPattern *pattern) {
    FcResult result;
    FcPattern *font = FcFontMatch(fontConfig, pattern, &result);
    if (!font) {
        throw std::runtime_error("No matching font");
    }

    FcChar8 *file;
    if (FcPatternGetString(font, FC_FILE, 0, &file) != FcResultMatch) {
        throw std::runtime_error("Failed to get font file");
    }

    std::string fontPath = reinterpret_cast<const char *>(file);

    FcPatternDestroy(font);

    return fontPath;
}

bool Font::checkCharInFont(unsigned long charCode) {
    FT_ULong charIndex = FT_Get_Char_Index(face, charCode);
    return charIndex != 0;
}

std::string Font::getFallbackFontPath(unsigned long charCode) {
    FcCharSet *charSet = FcCharSetCreate();
    FcCharSetAddChar(charSet, charCode);
    auto fontPatternCopy = FcPatternDuplicate(fontPattern);
    FcPatternAddCharSet(fontPatternCopy, FC_CHARSET, charSet);
    return getFontPathForPattern(fontPatternCopy);
}

void Font::loadCharacter(unsigned long charCode) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    FT_Face actualFace = face;
    if (!checkCharInFont(charCode)) {
        std::string fallbackFontPath = getFallbackFontPath(charCode);
        FT_Face fallbackFace;
        if (FT_New_Face(ft, fallbackFontPath.c_str(), 0, &fallbackFace)) {
            std::cerr << "ERROR::FREETYPE: Failed to load fallback font" << std::endl;
            return;
        }
        FT_Set_Pixel_Sizes(fallbackFace, 0, settings.fontSize);
        FT_ULong charIndex = FT_Get_Char_Index(fallbackFace, charCode);
        std::cout << "Character " << charCode
                  << " not found in font, using fallback font: " << fallbackFace->family_name << std::endl;
        std::cout << charIndex << std::endl;
        actualFace = fallbackFace;
    }

    if (FT_Load_Char(actualFace, charCode, FT_LOAD_RENDER)) {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return;
    }

    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        actualFace->glyph->bitmap.width,
        actualFace->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        actualFace->glyph->bitmap.buffer
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Glyph character = {
        texture,
        Vec2(actualFace->glyph->bitmap.width, actualFace->glyph->bitmap.rows),
        Vec2(actualFace->glyph->bitmap_left, actualFace->glyph->bitmap_top),
        face->glyph->advance.x
    };
    characters.insert(std::pair<unsigned long, Glyph>(charCode, character));
    glBindTexture(GL_TEXTURE_2D, 0);
}

Font::~Font() {
    if (glfwGetCurrentContext() != nullptr) {
        for (auto &pair : characters) {
            glDeleteTextures(1, &pair.second.textureID);
        }
    }
    FcPatternDestroy(fontPattern);
    FT_Done_Face(face);
}

FontManager::FontManager() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    freeType.reset(ft);

    fontConfig.reset(FcInitLoadConfigAndFonts());
    if (!fontConfig) {
        std::cerr << "Failed to initialize fontconfig" << std::endl;
        throw std::runtime_error("Failed to initialize fontconfig");
    }
}

Font *FontManager::getFont(FontSettings settings) {
    size_t hash = std::hash<std::string>()(settings.font) ^ std::hash<int>()(static_cast<int>(settings.slant)) ^
                  std::hash<int>()(static_cast<int>(settings.weight)) ^ std::hash<int>()(settings.fontSize);
    auto it = fonts.find(hash);
    if (it != fonts.end()) {
        return it->second.get();
    } else {
        FT_Library ft = freeType.get();
        auto font = std::make_unique<Font>(settings, fontConfig.get(), ft);
        fonts.insert({hash, std::move(font)});
        return fonts[hash].get();
    }
}

} // namespace gltk
