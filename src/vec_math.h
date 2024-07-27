#pragma once

#include <math.h>
#include <array>

#define PI 3.14159265359f

class Vec2 {
public:
    float x;
    float y;

    Vec2(float x, float y);
    Vec2();

    void asArray(float *dest);

    Vec2 operator+(const Vec2 b) const;
    Vec2 operator+=(const Vec2 b);

    Vec2 operator-(const Vec2 b) const;
    Vec2 operator-=(const Vec2 b);

    float Magnitude() const;

    Vec2 Noramlize();

    Vec2 operator*(const float b) const;
    Vec2 operator*=(const float b);

    float Dot(Vec2 b) const;

    float Angle(Vec2 b) const;

    Vec2 Rot(float rot) const;

    Vec2 Lerp(Vec2 b, float t) const;
};

class Mat3 {
public:
    std::array<float, 9> mat;
    
    Mat3();

    Mat3 operator*(const Mat3 &b) const;

    static Mat3 rotationMat(float angle);

    static Mat3 scalingMatrix(Vec2 scale);

    static Mat3 translationMatrix(Vec2 translate);

    void rotateMat(float angle);

    void scaleMat(Vec2 scale);

    void translateMat(Vec2 translate);
};
