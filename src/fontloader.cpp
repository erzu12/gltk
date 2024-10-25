#include "fontloader.h"


FontLoader::FontLoader() {
    // Initialize fontconfig
    config = FcInitLoadConfigAndFonts();
    if (!config) {
        std::cerr << "Failed to initialize fontconfig" << std::endl;
        throw std::runtime_error("Failed to initialize fontconfig");
    }
}

FontLoader::~FontLoader() {
    FcConfigDestroy(config);
}

std::string FontLoader::getFontPath(std::string fontName, FontSlant slant, FontWeight weight) {
    // Create a pattern to specify the font characteristics
    FcPattern* pat = FcNameParse((const FcChar8*)fontName.c_str());
    FcPatternAddInteger(pat, FC_SLANT, static_cast<int>(slant));
    FcPatternAddInteger(pat, FC_WEIGHT, static_cast<int>(weight));
    if (!pat) {
        throw std::runtime_error("Failed to create font pattern");
    }

    // Perform font matching
    FcConfigSubstitute(config, pat, FcMatchPattern);
    FcDefaultSubstitute(pat);

    FcResult result;
    FcPattern* font = FcFontMatch(config, pat, &result);
    if (!font) {
        throw std::runtime_error("No matching font found for: " + fontName);
    }

    // Get the matched font information
    FcChar8* file;
    if (FcPatternGetString(font, FC_FILE, 0, &file) != FcResultMatch) {
        throw std::runtime_error("Failed to get font file path for: " + fontName);
    }

    std::string fontPath = reinterpret_cast<const char*>(file);

    // Clean up resources
    FcPatternDestroy(font);
    FcPatternDestroy(pat);

    return fontPath;
}
