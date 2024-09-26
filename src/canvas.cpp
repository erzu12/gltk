#include "canvas.h"
#include <iostream>

namespace gltk {

PathObject::PathObject(std::vector<Vec2> points, Style style, bool closed) : points(points), style(style), closed(closed) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * points.size(), points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    BoundingBox bounds = BoundingBox::fromPoints(points);

    float vertices[] = {
        bounds.min.x, bounds.min.y,
        bounds.min.x, bounds.max.y,
        bounds.max.x, bounds.min.y,
        bounds.max.x, bounds.min.y,
        bounds.min.x, bounds.max.y,
        bounds.max.x, bounds.max.y
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}


void PathObject::render(Mat3 &viewMatrix) {
    std::cout << "rendering path" << std::endl;
    shader.use();
    shader.UniformVec3("color", style.color);
    shader.UniformMat3("transform", viewMatrix);

    //render polygon using stencel buffer
    glClear(GL_STENCIL_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilFunc(GL_ALWAYS, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, points.size());

    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glStencilFunc( GL_EQUAL, 0x1, 0x1 );
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_STENCIL_TEST);
}

PathObject::~PathObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}



void Canvas::addObject(std::unique_ptr<CanvasObject> object) {
    objects.push_back(std::move(object));
}

Vec2 Canvas::getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) {
    return size;
}

Canvas::Canvas(Style style, Vec2 size) : style(style), size(size) {
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

void Canvas::render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) {
    Mat3 viewMatrix = Mat3::viewMatrix(viewSize);
    shader.use();
    shader.UniformVec3("color", style.color);

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
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    viewMatrix.translateMatrix(Vec2(modelMatrix[0][2], modelMatrix[1][2]));
    viewMatrix.rotateMatrix(style.rotation);
    viewMatrix.translateMatrix(-size / 2.0f);
    for (auto &object : objects) {
        object->render(viewMatrix);
    }

    glDisable(GL_SCISSOR_TEST);
}

Canvas::~Canvas() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

};
