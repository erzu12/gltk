#include "messure.h"

std::ostream& operator<<(std::ostream& os, const Sizing& sizing) {
    switch (sizing) {
        case Sizing::Fixed: return os << "Fixed";
        case Sizing::Expand: return os << "Expand";
        case Sizing::Shrink: return os << "Shrink";
        case Sizing::Fit: return os << "Fit";
    }
}

Vec2 MessureVec2::resolve(Vec2 parentSize) {
    return Vec2(x->resolve(parentSize.x), y->resolve(parentSize.y));
}

int AbsoluteMessure::resolve(int parentSize) {
    return value;
}

int RelativeMessure::resolve(int parentSize) {
    return value * parentSize;
}
