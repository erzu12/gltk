#include "box.h"
#include <glad/glad.h>
#include <iostream>

namespace gltk {

Vec2 Box::getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) {
    return Vec2();
}

Box::Box(Vec3 color, float radius, float rotation) : color(color), radius(radius), rotation(rotation) {
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

void Box::render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) {
    Mat3 viewMatrix = Mat3::viewMatrix(viewSize);
    shader.use();
    shader.UniformVec3("color", color);
    
    modelMatrix = Mat3::translationMatrix(Vec2(modelMatrix[0][2], modelMatrix[1][2]));
    modelMatrix.rotateMatrix(rotation);
    modelMatrix.scaleMatrix(size);

    Mat3 transform = viewMatrix * modelMatrix;
    shader.UniformMat3("transform", transform);
    float clipedRadius = std::min(radius, std::min(size.x, size.y) / 2.0f);
    shader.UniformFloat("radius", clipedRadius);
    shader.UniformVec2("pixelSize", size);
    glEnable(GL_SCISSOR_TEST);
    glScissor(clipRegion.min.x, viewSize.y - clipRegion.max.y, clipRegion.width(), clipRegion.height());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_SCISSOR_TEST);
}

Box::~Box() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

};
