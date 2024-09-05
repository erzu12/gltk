#pragma once

#include "vec_math.h"

/*
 * Fixed: The size of the element is set by the size property.
 * Expand: The size of the element is expanded to include all of its children, if the children are larger than the element itself.
 * Shrink: The size of the element is shrunk to fit its children, if the children are smaller than the element itself.
 * Fit: The size of the element is adjusted to fit its children, ignoring the size of the element itself.
 *
 * Note: Children with relative sizes will use the original size of the element, not the adjusted size.
 */
enum class Sizing {
    Fixed,
    Expand,
    Shrink,
    Fit,
};

class IMessure {
    public:
        virtual int resolve(int parrentSize) = 0;
        virtual bool isAbsolute() = 0;
};

class AbsoluteMessure : public IMessure {
    private:
        int value;
    public:
        AbsoluteMessure(int value) : value(value) {}
        int resolve(int parrentSize) override;
        bool isAbsolute() override { return true; }
};

class RelativeMessure : public IMessure {
    private:
        float value;
    public:
        RelativeMessure(float value) : value(value) {}
        int resolve(int parrentSize) override;
        bool isAbsolute() override { return false; }
};

struct MessureVec2 {
    IMessure* x;
    IMessure* y;
    MessureVec2(IMessure* x, IMessure* y) : x(x), y(y) {}
    MessureVec2(double x, double y) : x(new RelativeMessure(x)), y(new RelativeMessure(y)) {}
    MessureVec2(int x, int y) : x(new AbsoluteMessure(x)), y(new AbsoluteMessure(y)) {}
    MessureVec2(double x, int y) : x(new RelativeMessure(x)), y(new AbsoluteMessure(y)) {}
    MessureVec2(int x, double y) : x(new AbsoluteMessure(x)), y(new RelativeMessure(y)) {}
    Vec2 resolve(Vec2 parrentSize);
};
