#include "../src/vec_math.h"
#include <gtest/gtest.h>

namespace gltk {

TEST(BoundsTest, boundsFromPositionSize) {
    BoundingBox bounds(Vec2(0, 0), Vec2(100, 100));
    ASSERT_EQ(bounds.min, Vec2(-50, -50));
    ASSERT_EQ(bounds.max, Vec2(50, 50));
}

TEST(BoundsTest, boundsAdd) {
    BoundingBox bounds1(Vec2(0, 0), Vec2(100, 100));
    BoundingBox bounds2(Vec2(100, 100), Vec2(100, 100));

    bounds1.add(bounds2);

    ASSERT_EQ(bounds1.min, Vec2(-50, -50));
    ASSERT_EQ(bounds1.max, Vec2(150, 150));
}

TEST(BoundsTest, boundsIntersect) {
    BoundingBox bounds1(Vec2(0, 0), Vec2(100, 100));
    BoundingBox bounds2(Vec2(50, 50), Vec2(100, 100));

    bounds1.intersect(bounds2);

    ASSERT_EQ(bounds1.min, Vec2(0, 0));
    ASSERT_EQ(bounds1.max, Vec2(50, 50));
}

} // namespace gltk
