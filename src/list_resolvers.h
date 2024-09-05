#pragma once
#include "vec_math.h"
#include "messure.h"

#include <vector>
#include <functional>

namespace gltk {

class layout_exception : public std::runtime_error {
    public:
        layout_exception(const std::string &msg) : std::runtime_error(msg) {}
};

enum class ListDirection {
    Down,
    Right,
    Left,
    Up,
};

struct ChildData {
    MessureVec2 *size;
    Sizing horizontalSizing;
    Sizing verticalSizing;
    std::function<BoundingBox(Vec2, Vec2, bool, ListDirection)> resolveTransform;
};

class ListResolver {
    void resolve(std::vector<ChildData> children);
};

class ListStrechResolver {
public:
    BoundingBox resolve(std::vector<ChildData> children, Vec2 resolvedSize);

    ListStrechResolver(ListDirection listDirection, Vec2 resolvedSize, Vec2 resolvedPosition) :
        listDirection(listDirection),
        resolvedSize(resolvedSize),
        resolvedPosition(resolvedPosition) {}

    Vec2 resolvedSize;
    Vec2 resolvedPosition;
private:
    ListDirection listDirection;

    void adjustCurrentPosition(Vec2 childSize, Vec2 &currentPosition);
    Vec2 getListParentSize(Vec2 childSize);
    Vec2 getListStartPossition();
    IMessure* getListDirectionMessure(MessureVec2 *messure);
    float &getListDirectionValue(Vec2 &vec);
    Sizing getListDirectionSizing(Sizing horizontalSizing, Sizing verticalSizing);
    Vec2 getChildMinSize(ChildData *child);
};


} // namespace gltk
