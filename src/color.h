#pragma once

#include <ostream>

namespace gltk {

class Color {
public:
    float r, g, b, a;

    Color() : r(0), g(0), b(0), a(1) {}
    Color(float rgb) : r(rgb), g(rgb), b(rgb), a(1) {}
    Color(float r, float g, float b) : r(r), g(g), b(b), a(1) {}
    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

    static Color fromHSVA(float h, float s, float v, float a);
    static Color fromHSV(float h, float s, float v);
    static Color fromHex(int hex);

    Color operator+(const Color other) const;
    Color operator+=(const Color other);

    Color operator-(const Color other) const;
    Color operator-=(const Color other);

    Color operator*(const float other) const;
    Color operator*=(const float other);
    Color operator*(const Color other) const;

    Color operator/(const float other) const;


    bool operator==(const Color &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Color &f2);
};

}  // namespace gltk
