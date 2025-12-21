#include "messure.h"

Vec2 MessureVec2::resolve(Vec2 parentSize) { return Vec2(x->resolve(parentSize.x), y->resolve(parentSize.y)); }

float AbsoluteMessure::resolve(float parentSize) { return value.get(); }

float RelativeMessure::resolve(float parentSize) { return value.get() * parentSize; }

std::unique_ptr<IMessure> &MessureVec2::operator[](int index) {
    if (index == 0) {
        return x;
    } else if (index == 1) {
        return y;
    } else {
        throw std::out_of_range("Index out of range for MessureVec2");
    }
}

AbsoluteMessure operator""_px(unsigned long long int value) { return AbsoluteMessure((int)value); }

RelativeMessure operator""_percent(long double value) { return RelativeMessure((float)value / 100.0f); }

RelativeMessure operator""_pct(long double value) { return RelativeMessure((float)value / 100.0f); }

RelativeMessure operator""_percent(unsigned long long int value) { return RelativeMessure((float)value / 100.0f); }

RelativeMessure operator""_pct(unsigned long long int value) { return RelativeMessure((float)value / 100.0f); }
