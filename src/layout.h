#pragma once

#include "vec_math.h"
#include <optional>
#include <vector>


class layout_exception : public std::runtime_error {
public:
    layout_exception(const std::string &msg) : std::runtime_error(msg) {}
};

namespace Anchors {
    const Vec2 TopLeft = Vec2(0, 0);
    const Vec2 TopCenter = Vec2(0.5, 0);
    const Vec2 TopRight = Vec2(1, 0);
    const Vec2 CenterLeft = Vec2(0, 0.5);
    const Vec2 Center = Vec2(0.5, 0.5);
    const Vec2 CenterRight = Vec2(1, 0.5);
    const Vec2 BottomLeft = Vec2(0, 1);
    const Vec2 BottomCenter = Vec2(0.5, 1);
    const Vec2 BottomRight = Vec2(1, 1);
}

class IMessure {
public:
    virtual int resolve(int parrentSize) = 0;
};

class AbsoluteMessure : public IMessure {
    private:
        int value;
    public:
        AbsoluteMessure(int value) : value(value) {}
        int resolve(int parrentSize) override;
};

class RelativeMessure : public IMessure {
    private:
        float value;
    public:
        RelativeMessure(float value) : value(value) {}
        int resolve(int parrentSize) override;
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

class Layout {
    Vec2 anchor = Anchors::TopLeft;
    MessureVec2 offset;
    Vec2 pivot = Anchors::TopLeft;
    MessureVec2 size;

    std::optional<Layout*> parent;
    std::vector<Layout*> children;
public:
    Layout(MessureVec2 viewportSize); // root layout defined by the window
    Layout(Layout *parrent, MessureVec2 position, MessureVec2 size);
    Layout(Layout *parrent, Vec2 anchor, MessureVec2 offset, Vec2 pivot, MessureVec2 size);

    void setSize(MessureVec2 size);

    void addChild(Layout *child);

    Mat3 resolveTransform();
    Vec2 resolveSize();
};