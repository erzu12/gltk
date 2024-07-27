#include "box.h"
#include <glad/glad.h>
#include <iostream>

namespace gltk {

Box::Box(Layout layout, float radius) : layout(layout), radius(radius) {
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
    shader.UniformVec3("color", Vec3(1.0f, 0.0f, 0.0f));
    Mat3 model = layout.resolveTransform(viewportSize);
    Mat3 view = Mat3::viewMatrix(viewportSize);
    Mat3 transform = view * model;
    shader.UniformMat3("transform", transform);
    Vec2 size = layout.resolveSize(viewportSize);
    float clipedRadius = std::min(radius, std::min(size.x, size.y) / 2.0f);
    shader.UniformFloat("radius", clipedRadius);
    shader.UniformVec2("pixelSize", layout.resolveSize(viewportSize));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

Box::~Box() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

};
