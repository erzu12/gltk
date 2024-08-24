#include <gtest/gtest.h>
#include "../src/vec_math.h"

namespace gltk {

TEST(BoundsTest, boundsFromPositionSizePivot) {
    Bounds bounds(Vec2(0, 0), Vec2(100, 100), Vec2(0.5, 0.5));
    ASSERT_EQ(bounds.min, Vec2(-50, -50));
    ASSERT_EQ(bounds.max, Vec2(50, 50));
}

TEST(BoundsTest, boundsFromPositionSizePivotTopLeft) {
    Bounds bounds(Vec2(0, 0), Vec2(100, 100), Vec2(0, 0));
    ASSERT_EQ(bounds.min, Vec2(0, 0));
    ASSERT_EQ(bounds.max, Vec2(100, 100));
}

TEST(BoundsTest, boundsFromPositionSizePivotBottomRight) {
    Bounds bounds(Vec2(0, 0), Vec2(100, 100), Vec2(1, 1));
    ASSERT_EQ(bounds.min, Vec2(-100, -100));
    ASSERT_EQ(bounds.max, Vec2(0, 0));
}

TEST(BoundsTest, boundsAdd) {
    Bounds bounds1(Vec2(-50, -50), Vec2(100, 100));
    Bounds bounds2(Vec2(0, 0), Vec2(150, 150));

    bounds1.add(bounds2);

    ASSERT_EQ(bounds1.min, Vec2(-50, -50));
    ASSERT_EQ(bounds1.max, Vec2(150, 150));
}

}  // namespace gltk

