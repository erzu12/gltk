#include <gtest/gtest.h>
#include "../src/layout.h"


TEST(LayoutTest, layoutTopLeftSquareAbsolute) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, MessureVec2(0, 0), MessureVec2(100, 100));
    Mat3 transform = layout.resolveTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 50.0f);
}

TEST(LayoutTest, layoutTopLeftRectangleAbsolute) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, MessureVec2(0, 0), MessureVec2(100, 200));
    Mat3 transform = layout.resolveTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 200.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutTopLeftSquareRelative) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, MessureVec2(0, 0), MessureVec2(0.5, 0.5));
    Mat3 transform = layout.resolveTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 50.0f);
}

TEST(LayoutTest, layoutTopLeftRectangleRelative) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, MessureVec2(0, 0), MessureVec2(0.5, 1.0));
    Mat3 transform = layout.resolveTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 200.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutTopLeftSquareMixed) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, MessureVec2(0, 0), MessureVec2(100, 0.5));
    Mat3 transform = layout.resolveTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 50.0f);
}

TEST(LayoutTest, layoutCenterAbsolute) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::Center, MessureVec2(0, 0), Anchors::Center, MessureVec2(50, 100));
    Mat3 transform = layout.resolveTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutCenterRelative) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::Center, MessureVec2(0, 0), Anchors::Center, MessureVec2(0.25, 0.5));
    Mat3 transform = layout.resolveTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutCenterPivotCenterRight) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::Center, MessureVec2(0, 0), Anchors::CenterRight, MessureVec2(100, 50));
    Mat3 transform = layout.resolveTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

