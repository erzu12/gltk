#include "messure.h"

std::ostream& operator<<(std::ostream& os, const Sizing& sizing) {
    using enum Sizing;
    switch (sizing) {
        case Fixed: return os << "Fixed";
        case Expand: return os << "Expand";
        case Shrink: return os << "Shrink";
        case Fit: return os << "Fit";
    }
    return os << "Unknown Sizing";
}

Vec2 MessureVec2::resolve(Vec2 parentSize) {
    return Vec2(x->resolve(parentSize.x), y->resolve(parentSize.y));
}

float AbsoluteMessure::resolve(float parentSize) {
    return (float)value;
}

float RelativeMessure::resolve(float parentSize) {
    return value * parentSize;
}

AbsoluteMessure operator"" _px(unsigned long long int value) {
    return AbsoluteMessure((int)value);
}

RelativeMessure operator"" _percent(long double value) {
    return RelativeMessure((float)value / 100.0f);
}

RelativeMessure operator"" _pct(long double value) {
    return RelativeMessure((float)value / 100.0f);
}

RelativeMessure operator"" _percent(unsigned long long int value) {
    return RelativeMessure((float)value / 100.0f);
}

RelativeMessure operator"" _pct(unsigned long long int value) {
    return RelativeMessure((float)value / 100.0f);
}
