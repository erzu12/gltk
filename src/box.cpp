#include "box.h"
#include <glad/glad.h>
#include <iostream>

namespace gltk {

Box::Box(std::unique_ptr<Layout> &layout, Vec3 color, float radius) : layout(std::move(layout)), color(color), radius(radius) {
    if (!this->layout) {
        throw std::invalid_argument("Layout cannot be null");
    }
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

void Box::draw(Vec2 viewportSize) {
    shader.use();
    shader.UniformVec3("color", color);
    Mat3 model = layout->getTransform();
    Mat3 view = Mat3::viewMatrix(viewportSize);
    Mat3 transform = view * model;
    shader.UniformMat3("transform", transform);
    Vec2 size = layout->getSize();
    float clipedRadius = std::min(radius, std::min(size.x, size.y) / 2.0f);
    shader.UniformFloat("radius", clipedRadius);
    shader.UniformVec2("pixelSize", layout->getSize());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Box::addChild(Box &child) {
    layout->addChild(child.layout.get());
}

Box::~Box() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

};
