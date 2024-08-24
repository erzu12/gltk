#pragma once

#include <math.h>
#include <array>
#include <ostream>

#define PI 3.14159265359f

class Vec2 {
public:
    float x;
    float y;

    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}

    void asArray(float *dest);

    Vec2 operator+(const Vec2 b) const;
    Vec2 operator+=(const Vec2 b);

    Vec2 operator-(const Vec2 b) const;
    Vec2 operator-=(const Vec2 b);

    float Magnitude() const;

    Vec2 Noramlize();

    Vec2 operator*(const float b) const;
    Vec2 operator*=(const float b);
    Vec2 operator*(const Vec2 b) const;

    Vec2 operator/(const float b) const;

    float Dot(const Vec2 b) const;

    float Angle(const Vec2 b) const;

    Vec2 Rot(float rot) const;

    Vec2 max(const Vec2 b);
    Vec2 min(const Vec2 b);

    Vec2 Lerp(Vec2 b, float t) const;

    bool operator==(const Vec2 &b) const;

    friend std::ostream &operator<<(std::ostream &os, const Vec2 &f2);
};

class Vec3 {
public:
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3 &b) const;
    Vec3 operator+=(const Vec3 &b);

    Vec3 operator-(const Vec3 &b) const;
    Vec3 operator-=(const Vec3 &b);

    Vec3 operator-() const;

    float magnitude() const;

    Vec3 normalize();

    Vec3 operator*(float b) const;
    Vec3 operator*=(float b);
    Vec3 operator*(const Vec3 &b) const;

    Vec3 operator/(float b) const;

    float dot(const Vec3 &b) const;

    Vec3 cross(const Vec3 &b) const;

    Vec3 reflect(const Vec3 &normal) const;

    Vec3 max(const Vec3 &b);
    Vec3 min(const Vec3 &b);

    Vec3 lerp(const Vec3 &b, float t) const;

    bool operator==(const Vec3 &b) const;

    friend std::ostream &operator<<(std::ostream &os, const Vec3 &f3);
};


class Mat3 {
public:
    std::array<float, 9> mat;
    
    Mat3();

    Mat3 operator*(const Mat3 &b) const;

    static Mat3 rotationMatrix(float angle);

    static Mat3 scalingMatrix(Vec2 scale);

    static Mat3 translationMatrix(Vec2 translate);

    static Mat3 viewMatrix(Vec2 viewSize);

    void rotateMatrix(float angle);

    void scaleMatrix(Vec2 scale);

    void translateMatrix(Vec2 translate);

    float *data();

    std::array<float, 3> operator[](int index) const;

    friend std::ostream &operator<<(std::ostream &os, const Mat3 &m);
};

class Bounds {
    public:
        Vec2 min;
        Vec2 max;

        Bounds() : min(Vec2(float(INFINITY), float(INFINITY))), max(Vec2(float(-INFINITY), float(-INFINITY))) {}
        Bounds(Vec2 min, Vec2 max) : min(min), max(max) {}
        Bounds(Vec2 position, Vec2 size, Vec2 pivot) : min(position - size * pivot), max(position + size * (Vec2(1, 1) - pivot)) {}
        void add(const Bounds &other);
        bool contains(Vec2 point);
};
