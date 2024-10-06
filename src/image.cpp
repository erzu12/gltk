#include "image.h"
#include <glad/glad.h>
#include <iostream>

#include <stb_image.h>

namespace gltk {

Vec2 Image::solveTriangle(float angle, float hypotenuse) {
    float x = cos(angle) * hypotenuse;
    float y = sin(angle) * hypotenuse;
    return Vec2(std::abs(x), std::abs(y));
}

Vec2 Image::getSize(Vec2 layoutSize, bool fixedX, bool fixedY) {
    Vec2 outSize = Vec2();
    if (fixedX && fixedY) {
        outSize = layoutSize;
    }
    else if (fixedX) {
        outSize = Vec2(layoutSize.x, layoutSize.x * height / width);
    }
    else if (fixedY) {
        outSize = Vec2(layoutSize.y * width / height, layoutSize.y);
    }
    return outSize;
}

Image::Image(std::string path, Style style) : style(style) {
    float vertices[] = {
        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, -0.5f,
        0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    loadImage(path);
}

void Image::render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) {
    Mat3 viewMatrix = Mat3::viewMatrix(viewSize);
    shader.use();
    shader.UniformColor("color", style.color);

    modelMatrix = Mat3::translationMatrix(Vec2(modelMatrix[0][2], modelMatrix[1][2]));
    modelMatrix.rotateMatrix(style.rotation);
    modelMatrix.scaleMatrix(size);

    Mat3 transform = viewMatrix * modelMatrix;
    shader.UniformMat3("transform", transform);
    float clipedRadius = std::min(style.radius, std::min(size.x, size.y) / 2.0f);
    shader.UniformFloat("radius", clipedRadius);
    shader.UniformVec2("pixelSize", size);
    glEnable(GL_SCISSOR_TEST);
    glScissor(clipRegion.min.x, viewSize.y - clipRegion.max.y, clipRegion.width(), clipRegion.height());
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_SCISSOR_TEST);
}

void Image::loadImage(std::string path)
{
    int nrChannels;
    unsigned char *data;
    int glFormat = GL_RGBA;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    switch (nrChannels) {
        case 1:
            glFormat = GL_RED;
            break;
        case 3:
            glFormat = GL_RGB;
            break;
        case 4:
            glFormat = GL_RGBA;
            break;
        default:
            throw std::runtime_error("Unsupported number of color channels: " + std::to_string(nrChannels) + " in image: " + path);
    }

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else {
        throw std::runtime_error("Failed to load image: " + path);
    }

    shader.use();
    shader.UniformInt("text", 0);
}


Image::~Image() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

};
