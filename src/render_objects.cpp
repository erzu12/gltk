#include "render_objects.h"
#include <stb_image.h>
#include <iostream>

namespace gltk {

namespace {

void createQuad(unsigned int &VAO, unsigned int &VBO) {
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
}

void renderQuad(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion, Style style, unsigned int VAO, const Shader &shader) {
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
    glScissor(clipRegion.min.x, clipRegion.min.y, clipRegion.width(), clipRegion.height());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_SCISSOR_TEST);
}

void setImageData(unsigned int texture, int width, int height, int nrChannels, uint8_t *data) {
    int glFormat = GL_RGBA;
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
            throw std::runtime_error("Unsupported number of color channels: " + std::to_string(nrChannels));
    }

    glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

unsigned int loadImage(std::string path, int *width, int *height)
{
    unsigned int image;
    int nrChannels;
    unsigned char *data;

    glGenTextures(1, &image);
    glBindTexture(GL_TEXTURE_2D, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load(path.c_str(), width, height, &nrChannels, 0);

    if (data) {
        setImageData(image, *width, *height, nrChannels, data);
    }
    else {
        throw std::runtime_error("Failed to load image: " + path);
    }
    stbi_image_free(data);

    return image;
}

} // namespace


BoxRenderer::BoxRenderer() {
    createQuad(VAO, VBO);
}

void BoxRenderer::render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion, Style style) {
    renderQuad(viewSize, modelMatrix, size, clipRegion, style, VAO, shader);
}

BoxRenderer::~BoxRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

ImageRenderer::ImageRenderer(std::string path) {
    createQuad(VAO, VBO);
    texture = loadImage(path, &width, &height);
    shader.use();
    shader.UniformInt("image", 0);
}

ImageRenderer::ImageRenderer() {
    createQuad(VAO, VBO);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    shader.use();
    shader.UniformInt("image", 0);
}

void ImageRenderer::render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion, Style style) {
    if (width >= 0 || height >= 0) {
        glBindTexture(GL_TEXTURE_2D, texture);
        renderQuad(viewSize, modelMatrix, size, clipRegion, style, VAO, shader);
    }
    else {
        std::cout << "Image not loaded" << std::endl;
    }
}

void ImageRenderer::updateImage(uint8_t *data, int width, int height, int nrChannels) {
    glBindTexture(GL_TEXTURE_2D, texture);
    setImageData(texture, width, height, nrChannels, data);
    this->width = width;
    this->height = height;
}

ImageRenderer::~ImageRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

}
