#include "color.h"

#include <iostream>
#include <cmath>

namespace gltk {

Color Color::fromHSVA(float h, float s, float v, float a) {
    return fromHSV(h, s, v) + Color(0, 0, 0, a);
}

Color Color::fromHSV(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - std::abs(std::fmod(h / 60, 2) - 1));
    float m = v - c;

    float r, g, b;
    switch ((int)std::floor(h / 60) % 6) {
        case 0:
            r = c;
            g = x;
            b = 0;
            break;
        case 1:
            r = x;
            g = c;
            b = 0;
            break;
        case 2:
            r = 0;
            g = c;
            b = x;
            break;
        case 3:
            r = 0;
            g = x;
            b = c;
            break;
        case 4:
            r = x;
            g = 0;
            b = c;
            break;
        case 5:
            r = c;
            g = 0;
            b = x;
            break;
    }
    return Color(r + m, g + m, b + m);
}

Color Color::fromHex(int hex) {
    float r = ((hex >> 16) & 0xFF) / 255.0f;
    float g = ((hex >> 8) & 0xFF) / 255.0f;
    float b = (hex & 0xFF) / 255.0f;
    return Color(r, g, b);
}

Color Color::operator+(const Color other) const {
    return Color(r + other.r, g + other.g, b + other.b, a + other.a);
}

Color Color::operator+=(const Color other) {
    r += other.r;
    g += other.g;
    b += other.b;
    a += other.a;
    return *this;
}

Color Color::operator-(const Color other) const {
    return Color(r - other.r, g - other.g, b - other.b, a - other.a);
}

Color Color::operator-=(const Color other) {
    r -= other.r;
    g -= other.g;
    b -= other.b;
    a -= other.a;
    return *this;
}

Color Color::operator*(const float scalar) const {
    return Color(r * scalar, g * scalar, b * scalar, a * scalar);
}

Color Color::operator*=(const float scalar) {
    r *= scalar;
    g *= scalar;
    b *= scalar;
    a *= scalar;
    return *this;
}

Color Color::operator*(const Color other) const {
    return Color(r * other.r, g * other.g, b * other.b, a * other.a);
}

Color Color::operator/(const float scalar) const {
    return Color(r / scalar, g / scalar, b / scalar, a / scalar);
}

bool Color::operator==(const Color &other) const {
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

std::ostream &operator<<(std::ostream &os, const Color &f4) {
    os << "Color(" << f4.r << ", " << f4.g << ", " << f4.b << ", " << f4.a << ")";
    return os;
}

}  // namespace gltk
