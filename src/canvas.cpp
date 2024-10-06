#include "canvas.h"
#include <iostream>
#include <cassert>

namespace gltk {

PathObject::PathObject(std::vector<Vec2> points, Vec2 pos, Style style, bool interpolate, bool closed) : points(points), pos(pos), interpolate(interpolate), style(style), closed(closed) {
    if (interpolate) {
        this->points = bezierInterpolation(points);
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &borderVAO);
    glGenBuffers(1, &borderVBO);
    glGenBuffers(1, &borderEBO);
    
    glBindVertexArray(borderVAO);
    glBindBuffer(GL_ARRAY_BUFFER, borderVBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void PathObject::regenerateGeometry() {
    std::vector<Vec2> absolutePoints;
    absolutePoints.reserve(points.size());
    for (auto &point : points) {
        absolutePoints.push_back(point + pos);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * absolutePoints.size(), absolutePoints.data(), GL_DYNAMIC_DRAW);
    boundingBox = generateBorder(absolutePoints, style.borderWidth, closed);

    float vertices[] = {
        boundingBox.min.x, boundingBox.min.y,
        boundingBox.min.x, boundingBox.max.y,
        boundingBox.max.x, boundingBox.min.y,
        boundingBox.max.x, boundingBox.min.y,
        boundingBox.min.x, boundingBox.max.y,
        boundingBox.max.x, boundingBox.max.y
    };

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    dirty = false;
}

std::vector<Vec2> PathObject::bezierInterpolation(std::vector<Vec2> points) {
    assert(points.size() % 4 == 0);
    std::vector<Vec2> interpolated;
    int resolution = 100;

    for (int i = 0; i < points.size(); i += 4) {
        Vec2 start = points[i];
        Vec2 control1 = points[i + 1];
        Vec2 control2 = points[i + 2];
        Vec2 end = points[i + 3];

        int steps = resolution;
        if (i == points.size() - 4) {
            steps = resolution + 1;
        }
        for (int j = 0; j < steps; j++) {
            float t = (float)j / resolution;
            Vec2 a = start.lerp(control1, t);
            Vec2 b = control1.lerp(control2, t);
            Vec2 c = control2.lerp(end, t);
            Vec2 point = a.lerp(b, t).lerp(b.lerp(c, t), t);
            interpolated.push_back(point);
        }
    }
    return interpolated;
}

BoundingBox PathObject::generateBorder(std::vector<Vec2> points, float width, bool closed) {
    std::vector<Vec2> verts(points.size() * 2);
    borderIndCount = 6 * (points.size() - 1);
    if(closed) {
        borderIndCount += 6;
    }
    std::vector<unsigned int> inds(borderIndCount);

    Vec2 current;
    if (closed) {
        current = (points[0] - points.back()).Normalize();
        if (current == Vec2(0, 0)) {
            current = (points[0] - points[points.size() - 2]).Normalize();
        }
    }
    else {
        current = (points[1] - points[0]).Normalize();
    }

    for(int i = 0; i < points.size(); i++) {
        Vec2 next;
        if(i == points.size() - 1) {
            if (closed) {
                next = (points[0] - points[i]).Normalize();
            }
            else {
                next = current;
            }
        }
        else {
            next = (points[i + 1] - points[i]).Normalize();
        }
        float angle = (current.Angle(next)) / 2;
        float dist = 1.0f / cos(angle);
        Vec2 p1 = points[i] + current.Rot(PI / 2 + angle) * width / 2 * dist;
        Vec2 p2 = points[i] + current.Rot(-PI / 2 + angle) * width / 2 * dist;

        verts[i * 2] = p1;
        verts[i * 2 + 1] = p2;

        current = next;
    }

    for(int i = 1; i < points.size(); i++) {
        inds[i * 6 - 6] = i * 2 - 2;
        inds[i * 6 - 5] = i * 2 - 1;
        inds[i * 6 - 4] = i * 2;
        inds[i * 6 - 3] = i * 2 - 1;
        inds[i * 6 - 2] = i * 2 + 1;
        inds[i * 6 - 1] = i * 2;
    }

    if(closed) {
        inds[borderIndCount - 6] = 0;
        inds[borderIndCount - 5] = 1;
        inds[borderIndCount - 4] = points.size() * 2 - 2;
        inds[borderIndCount - 3] = 1;
        inds[borderIndCount - 2] = points.size() * 2 - 1;
        inds[borderIndCount - 1] = points.size() * 2 - 2;
    }


    glBindVertexArray(borderVAO);

    glBindBuffer(GL_ARRAY_BUFFER, borderVBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vec2), verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, borderEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, borderIndCount * sizeof(unsigned int), inds.data(), GL_STATIC_DRAW);

    return BoundingBox::fromPoints(verts);
}

void PathObject::render(Mat3 &viewMatrix) {
    if (dirty) {
        regenerateGeometry();
    }
    shader.use();
    shader.UniformColor("color", style.color);
    shader.UniformMat3("transform", viewMatrix);

    //Using a triangle fan to triangulate the polygon causes points outside the polygon to be covered 
    //by an evenen amount of triangles and points inside the polygon to be covered by an odd amount of triangles.
    //Setting the stencil operation to invert will cause the stencil buffer to be set to 1 for points inside the polygon
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

    // stencil for border
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilFunc(GL_ALWAYS, 0x1, 0x1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glBindVertexArray(borderVAO);
    glDrawElements(GL_TRIANGLES, borderIndCount, GL_UNSIGNED_INT, 0);

    // draw border
    shader.UniformColor("color", style.borderColor);
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glStencilFunc( GL_EQUAL, 0x1, 0x1 );
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisable(GL_STENCIL_TEST);
}

bool PathObject::pointInObject(Vec2 point) {
    if (dirty) {
        regenerateGeometry();
    }
    if (!boundingBox.contains(point)) {
        return false;
    }
    int intersectCount = 0;
    if (style.color.a > 0.000001) {
        for(int i = 2; i < points.size(); i++) {
            Vec2 a = points[0] + pos;
            Vec2 b = points[i - 1] + pos;
            Vec2 c = points[i] + pos;

            if (point.isInsideTriangle(a, b, c)) {
                intersectCount++;
            }
        }
    }
    if (intersectCount % 2 == 1) {
        return true;
    }
    if (style.borderWidth > 0.000001 && style.borderColor.a > 0.000001) {
        for(int i = 1; i < points.size(); i++) {
            Vec2 tri1a = borderTriangles[i * 2 - 2];
            Vec2 tri1b = borderTriangles[i * 2 - 1];
            Vec2 tri1c = borderTriangles[i * 2];
            Vec2 tri2a = borderTriangles[i * 2 - 1];
            Vec2 tri2b = borderTriangles[i * 2 + 1];
            Vec2 tri2c = borderTriangles[i * 2];

            bool res1 = point.isInsideTriangle(tri1a, tri1b, tri1c);
            bool res2 = point.isInsideTriangle(tri2a, tri2b, tri2c);

            if (res1 || res2) {
                return true;
            }
        }
    }
    return false;
}

void PathObject::rotate(float angle) {
    for (auto &point : points) {
        point = point.Rot(angle);
    }
    dirty = true;
}

void PathObject::translate(Vec2 pos) {
    this->pos += pos;
    dirty = true;
}

void PathObject::scale(Vec2 scale) {
    for (auto &point : points) {
        point = point * scale;
    }
    dirty = true;
}


PathObject::~PathObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Canvas::addObject(std::unique_ptr<CanvasObject> object) {
    objects.push_back(std::move(object));
}

Rectangle::Rectangle(Vec2 pos, Vec2 size, Style style) : PathObject({
    -size / 2.0f,
    Vec2(size.x, -size.y) / 2.0f,
    size / 2.0f,
    Vec2(-size.x, size.y) / 2.0f
}, pos, style, false, true) {}

Oval::Oval(Vec2 pos, Vec2 size, Style style) : PathObject([&] {
    // bezier curves cant be used to draw a perfect circle but a very close approximation is possible
    // see https://spencermortensen.com/articles/bezier-circle/ for more information
    float a = 1.00005519;
    float b = 0.55342686;
    float c = 0.99873585;

    std::vector<Vec2> newPints {
        Vec2(0, a),
        Vec2(b, c),
        Vec2(c, b),
        Vec2(a, 0),
        Vec2(a, 0),
        Vec2(c, -b),
        Vec2(b, -c),
        Vec2(0, -a),
        Vec2(0, -a),
        Vec2(-b, -c),
        Vec2(-c, -b),
        Vec2(-a, 0),
        Vec2(-a, 0),
        Vec2(-c, b),
        Vec2(-b, c),
        Vec2(0, a)
    };

    for (auto &point : newPints) {
        point = point * size / 2.0f;
    }

    return newPints;
}(), pos, style, true, true) {}

Vec2 Canvas::getSize(Vec2 LayoutSize, bool fixedX, bool fixedY) {
    return size;
}

Canvas::Canvas(Style style, Vec2 size) : style(style), size(size), boxRenderer() {
}

void Canvas::render(Vec2 viewSize, Mat3 &modelMatrix, Vec2 size, BoundingBox clipRegion) {
    boxRenderer.render(viewSize, modelMatrix, size, clipRegion, style);
    Mat3 viewMatrix = Mat3::viewMatrix(viewSize);
    viewMatrix.translateMatrix(Vec2(modelMatrix[0][2], modelMatrix[1][2]));
    viewMatrix.rotateMatrix(style.rotation);
    viewMatrix.translateMatrix(-size / 2.0f);

    Vec2 pos = Vec2(modelMatrix[0][2], modelMatrix[1][2]);
    BoundingBox canvasBounds(pos - size / 2.f, pos + size / 2.f);
    clipRegion.intersect(canvasBounds);
    glScissor(clipRegion.min.x, viewSize.y - clipRegion.max.y, clipRegion.width(), clipRegion.height());

    for (auto &object : objects) {
        object->render(viewMatrix);
    }

    glDisable(GL_SCISSOR_TEST);
}

};
