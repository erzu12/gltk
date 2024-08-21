#pragma once

#include "shader.h"
#include "render.h"

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace gltk {

struct Character {
    unsigned int textureID;
    Vec2 size;
    Vec2 bearing;
    long advance;
};

class Text : public IRenderable {
    Vec3 color;
    std::string text;
    const Shader shader = Shader("assets/text.vert", "assets/text.frag");
    unsigned int VAO, VBO;

    std::map<char, Character> Characters;

    void loadCharacters();
    Vec2 getRenderdSize();
    float getBearing();
public:
    Text(Vec3 color, std::string text);
    ~Text();

    void render(const Mat3 &viewMatrix, Mat3 &modelMatrix, Vec2 size) override;
};

}
