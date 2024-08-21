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
    Vec2 renderdSize = getRenderdSize();
    Vec2 inPos = Vec2(modelMatrix[0][2], modelMatrix[1][2]);
    Vec2 startPos = getStartPos(inPos, size);
    shader.UniformVec3("color", color);
    for (char c : text) {
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
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Vec2 Text::getRenderdSize() {
    float x = 0;
    float y = 0;
    for (char c : text) {
        Character ch = Characters[c];
        x += (ch.advance >> 6);
        y = std::max(y, ch.size.y);
    }
    return Vec2(x, y);
}

float Text::getBearing() {
    float bearing = 0;
    for (char c : text) {
        Character ch = Characters[c];
        bearing = std::max(bearing, ch.bearing.y);
    }
    return bearing;
}

Vec2 Text::getStartPos(Vec2 inPos, Vec2 boxSize) {
    Vec2 renderdSize = getRenderdSize();
    Vec2 startPos = inPos;
    startPos.y += getBearing();
    switch (horizontalAlign) {
        case HorizontalTextAlign::Left:
            startPos.x -= boxSize.x / 2;
            break;
        case HorizontalTextAlign::Center:
            startPos.x -= renderdSize.x / 2;
            break;
        case HorizontalTextAlign::Right:
            startPos.x -= renderdSize.x - boxSize.x / 2;
            break;
    }

    switch (verticalAlign) {
        case VerticalTextAlign::Top:
            startPos.y -= boxSize.y / 2;
            break;
        case VerticalTextAlign::Center:
            startPos.y -= renderdSize.y / 2;
            break;
        case VerticalTextAlign::Bottom:
            startPos.y -= renderdSize.y - boxSize.y / 2;
            break;
    }
    return startPos;
}

Text::~Text() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

};
