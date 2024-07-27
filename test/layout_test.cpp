#include <gtest/gtest.h>
#include "../src/layout.h"


TEST(LayoutTest, layoutTopLeftSquareAbsolute) {
    Layout layout(MessureVec2(0, 0), MessureVec2(100, 100));
    Mat3 transform = layout.resolveTransform(Vec2(200, 200));
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 50.0f);
}

TEST(LayoutTest, layoutTopLeftRectangleAbsolute) {
    Layout layout(MessureVec2(0, 0), MessureVec2(100, 200));
    Mat3 transform = layout.resolveTransform(Vec2(200, 200));
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 200.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutTopLeftSquareRelative) {
    Layout layout(MessureVec2(0, 0), MessureVec2(0.5, 0.5));
    Mat3 transform = layout.resolveTransform(Vec2(200, 200));
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 50.0f);
}

TEST(LayoutTest, layoutTopLeftRectangleRelative) {
    Layout layout(MessureVec2(0, 0), MessureVec2(0.5, 1.0));
    Mat3 transform = layout.resolveTransform(Vec2(200, 200));
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 200.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutTopLeftSquareMixed) {
    Layout layout(MessureVec2(0, 0), MessureVec2(100, 0.5));
    Mat3 transform = layout.resolveTransform(Vec2(200, 200));
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 50.0f);
}

TEST(LayoutTest, layoutCenterAbsolute) {
    Layout layout(Anchors::Center, MessureVec2(0, 0), Anchors::Center, MessureVec2(50, 100));
    Mat3 transform = layout.resolveTransform(Vec2(200, 200));
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutCenterRelative) {
    Layout layout(Anchors::Center, MessureVec2(0, 0), Anchors::Center, MessureVec2(0.25, 0.5));
    Mat3 transform = layout.resolveTransform(Vec2(200, 200));
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutCenterPivotCenterRight) {
    Layout layout(Anchors::Center, MessureVec2(0, 0), Anchors::CenterRight, MessureVec2(100, 50));
    Mat3 transform = layout.resolveTransform(Vec2(200, 200));
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}
