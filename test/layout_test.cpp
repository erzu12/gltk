#include <gtest/gtest.h>
#include "../src/layout.h"

namespace gltk {


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

TEST(LayoutTest, layoutListStretchEvenSplit) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch);
    Layout child1(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(0.5, 1.0));
    Layout child2(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(0.5, 1.0));
    rootLayout.resolveTransform();
    Mat3 transform1 = child1.getTransform();
    Mat3 transform2 = child2.getTransform();
    Mat3 listTransform = listLayout.getTransform();
    std::cout << listTransform << std::endl;
    std::cout << transform1 << std::endl;
    std::cout << transform2 << std::endl;
    // Child 1 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform1[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform1[0][2], 25.0f);
    // Child 2 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform2[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform2[0][2], 75.0f);
}

TEST(LayoutTest, layoutListStretch3UnevenSplit) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch);
    Layout child1(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(0.5, 1.0));
    Layout child2(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(0.25, 1.0));
    Layout child3(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(0.25, 1.0));
    rootLayout.resolveTransform();
    Mat3 transform1 = child1.getTransform();
    Mat3 transform2 = child2.getTransform();
    Mat3 transform3 = child3.getTransform();
    Mat3 listTransform = listLayout.getTransform();
    std::cout << listTransform << std::endl;
    std::cout << transform1 << std::endl;
    std::cout << transform2 << std::endl;
    std::cout << transform3 << std::endl;
    // Child 1 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform1[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform1[0][2], 25.0f);
    // Child 2 should be 1/4 of the space
    ASSERT_FLOAT_EQ(transform2[0][0], 25.0f);
    ASSERT_FLOAT_EQ(transform2[0][2], 62.5f);
    // Child 3 should be 1/4 of the space
    ASSERT_FLOAT_EQ(transform3[0][0], 25.0f);
    ASSERT_FLOAT_EQ(transform3[0][2], 87.5f);
}

TEST(LayoutTest, layoutListAbsoluteAndRelative) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch);
    Layout child1(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(40, 1.0));
    Layout child2(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, 1.0));
    rootLayout.resolveTransform();
    Mat3 transform1 = child1.getTransform();
    Mat3 transform2 = child2.getTransform();

    // Child 1 should be 40 pixels wide
    ASSERT_FLOAT_EQ(transform1[0][0], 40.0f);
    ASSERT_FLOAT_EQ(transform1[0][2], 20.0f);
    // Child 2 should be 60 pixels wide
    ASSERT_FLOAT_EQ(transform2[0][0], 60.0f);
    ASSERT_FLOAT_EQ(transform2[0][2], 70.0f);
}

TEST(LayoutTest, layoutListRelativeAndAbsolute) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch);
    Layout child1(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, 1.0));
    Layout child2(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(40, 1.0));
    rootLayout.resolveTransform();
    Mat3 transform1 = child1.getTransform();
    Mat3 transform2 = child2.getTransform();

    std::cout << transform1 << std::endl;
    std::cout << transform2 << std::endl;

    // Child 1 should be 60 pixels wide
    ASSERT_FLOAT_EQ(transform1[0][0], 60.0f);
    ASSERT_FLOAT_EQ(transform1[0][2], 30.0f);
    // Child 2 should be 40 pixels wide
    ASSERT_FLOAT_EQ(transform2[0][0], 40.0f);
    ASSERT_FLOAT_EQ(transform2[0][2], 80.0f);
}

TEST(LayoutTest, layoutListMultiAbsolutAndRelative) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch);
    Layout child1(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(20, 1.0));
    Layout child2(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.8, 1.0));
    Layout child3(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(30, 1.0));
    Layout child4(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(1.2, 1.0));
    rootLayout.resolveTransform();
    Mat3 transform1 = child1.getTransform();
    Mat3 transform2 = child2.getTransform();
    Mat3 transform3 = child3.getTransform();
    Mat3 transform4 = child4.getTransform();

    // Child 1 should be 20 pixels wide
    ASSERT_FLOAT_EQ(transform1[0][0], 20.0f);
    ASSERT_FLOAT_EQ(transform1[0][2], 10.0f);
    // Child 2 should be 20 pixels wide
    ASSERT_FLOAT_EQ(transform2[0][0], 20.0f);
    ASSERT_FLOAT_EQ(transform2[0][2], 30.0f);
    // Child 3 should be 30 pixels wide
    ASSERT_FLOAT_EQ(transform3[0][0], 30.0f);
    ASSERT_FLOAT_EQ(transform3[0][2], 55.0f);
    // Child 4 should be 30 pixels wide
    ASSERT_FLOAT_EQ(transform4[0][0], 30.0f);
    ASSERT_FLOAT_EQ(transform4[0][2], 85.0f);
}

}  // namespace gltk
