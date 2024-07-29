#include <gtest/gtest.h>
#include "../src/layout.h"


TEST(LayoutTest, layoutTopLeftSquareAbsolute) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 100));
    rootLayout.resolveTransform();
    Mat3 transform = layout.getTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 50.0f);
}

TEST(LayoutTest, layoutTopLeftRectangleAbsolute) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 200));
    rootLayout.resolveTransform();
    Mat3 transform = layout.getTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 200.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutTopLeftSquareRelative) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(0.5, 0.5));
    rootLayout.addChild(&layout);
    rootLayout.resolveTransform();
    Mat3 transform = layout.getTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 50.0f);
}

TEST(LayoutTest, layoutTopLeftRectangleRelative) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(0.5, 1.0));
    rootLayout.resolveTransform();
    Mat3 transform = layout.getTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 200.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutTopLeftSquareMixed) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, .5));
    rootLayout.resolveTransform();
    Mat3 transform = layout.getTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 50.0f);
}

TEST(LayoutTest, layoutCenterAbsolute) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::Center, MessureVec2(0, 0), Anchors::Center, MessureVec2(50, 100));
    rootLayout.resolveTransform();
    Mat3 transform = layout.getTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutCenterRelative) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::Center, MessureVec2(0, 0), Anchors::Center, MessureVec2(0.25, 0.5));
    rootLayout.resolveTransform();
    Mat3 transform = layout.getTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutCenterPivotCenterRight) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::Center, MessureVec2(0, 0), Anchors::CenterRight, MessureVec2(100, 50));
    rootLayout.resolveTransform();
    Mat3 transform = layout.getTransform();
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
}

TEST(LayoutTest, layoutInheritPositionAndSize) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::BottomRight, MessureVec2(0, 0), Anchors::BottomRight, MessureVec2(.5, .5));
    Layout childLayout(&layout, Anchors::BottomRight, MessureVec2(-10, -20), Anchors::BottomRight, MessureVec2(.5, .5));
    rootLayout.resolveTransform();
    Mat3 transform = childLayout.getTransform();
    Mat3 parentTransform = layout.getTransform();
    std::cout << parentTransform << std::endl;
    std::cout << transform << std::endl;
    ASSERT_FLOAT_EQ(transform[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform[0][2], 165.0f);
    ASSERT_FLOAT_EQ(transform[1][2], 155.0f);

}

TEST(LayoutTest, unresolvedLayoutTrowsOnGetTransform) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout layout(&rootLayout, Anchors::BottomRight, MessureVec2(0, 0), Anchors::BottomRight, MessureVec2(.5, .5));
    ASSERT_THROW(layout.getTransform(), std::bad_optional_access);
}
