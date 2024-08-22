#include "text.h"
#include <glad/glad.h>
#include <iostream>

namespace gltk {


Text::Text(std::string text, int fontSize, std::string font, Vec3 color, HorizontalTextAlign horizontalAlign, VerticalTextAlign verticalAlign) : 
    text(text), fontSize(fontSize), color(color), horizontalAlign(horizontalAlign), verticalAlign(verticalAlign) {
    float vertices[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    loadCharacters(font, fontSize);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Text::render(const Mat3 &viewMatrix, Mat3 &modelMatrix, Vec2 size) {
    shader.use();
    auto lines = splitTextToLines(text, size.x);
    Vec2 renderdSize = getRenderdSize(lines);
    Vec2 inPos = Vec2(modelMatrix[0][2], modelMatrix[1][2]);
    Vec2 startPos;
    startPos.y = getVerticalStartPos(inPos.y, size.y, lines);
    shader.UniformVec3("color", color);
    for (std::string line : lines) {
        startPos.x = getHorizontalStartPos(inPos.x, size.x, line);
        for (char c : line) {
            Character ch = Characters[c];

            float xpos = startPos.x + ch.bearing.x;
            float ypos = startPos.y - ch.bearing.y;

            float w = ch.size.x;
            float h = ch.size.y;

            Mat3 characterModelMat = Mat3::translationMatrix(Vec2(xpos, ypos));
            characterModelMat.scaleMatrix(Vec2(w, h));

            Mat3 transform = viewMatrix * characterModelMat;
            shader.UniformMat3("transform", transform);
            shader.UniformVec2("pixelSize", size);
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            startPos.x += (ch.advance >> 6);
        }
        startPos.y += fontSize * lineHeight;
    }
}

void Text::loadCharacters(std::string font, int fontSize) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    FT_Face face;
    if (FT_New_Face(ft, ("/usr/share/fonts/TTF/" + font + ".TTF").c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
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
        Character character = {
            texture, 
            Vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        verticalBearing = std::max(verticalBearing, face->glyph->bitmap_top);
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

std::vector<std::string> Text::splitTextToLines(std::string text, float width) {
    std::string line = "";
    std::string word = "";
    std::vector<std::string> lines;
    for (char c : text) {
        if (c == ' ' || c == '\n') {
            if (!line.empty() && getRenderdLineWidht(line + word) > width) {
                lines.push_back(line);
                line = word.substr(1);
                word = " ";
            } else {
                line += word;
                word = " ";
            }
        } 
        if (c == '\n') {
            lines.push_back(line);
            line = "";
            word = "";
        } 
        if (c != ' ' && c != '\n') {
            word += c;
        }
    }
    if (!line.empty() && getRenderdLineWidht(line + word) > width) {
        lines.push_back(line);
        lines.push_back(word.substr(1));
    } else {
        lines.push_back(line + word);
    }
    return lines;
}

int Text::getRenderdLineWidht(std::string line) {
    int width = 0;
    for (char c : line) {
        Character ch = Characters[c];
        width += (ch.advance >> 6);
    }
    return width;
}

Vec2 Text::getRenderdSize(std::vector<std::string> lines) {
    float x = 0;
    float y = 0;
    for (std::string line : lines) {
        x = std::max(x, (float)getRenderdLineWidht(line));
        y += fontSize * lineHeight;
    }
    return Vec2(x, y);
}

float Text::getHorizontalStartPos(float inPos, float boxSize, std::string line) {
    float renderdLineWidht = getRenderdLineWidht(line);
    switch (horizontalAlign) {
        case HorizontalTextAlign::Left:
            return inPos - boxSize / 2;
        case HorizontalTextAlign::Center:
            return inPos - renderdLineWidht / 2;
        case HorizontalTextAlign::Right:
            return inPos + boxSize / 2 - renderdLineWidht;
    }
    return 0;
}

float Text::getVerticalStartPos(float inPos, float boxSize, std::vector<std::string> lines) {
    Vec2 renderdSize = getRenderdSize(lines);
    float startPos = inPos;
    startPos += verticalBearing;

    switch (verticalAlign) {
        case VerticalTextAlign::Top:
            startPos -= boxSize / 2;
            break;
        case VerticalTextAlign::Center:
            startPos -= renderdSize.y / 2;
            break;
        case VerticalTextAlign::Bottom:
            startPos -= renderdSize.y - boxSize / 2;
            break;
    }
    return startPos;
}

Text::~Text() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

};
