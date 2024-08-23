#include <gtest/gtest.h>
#include "../src/layout.h"

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
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch, ListDirection::Right);
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
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch, ListDirection::Right);
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

TEST(LayoutTest, layoutListStretchAbsoluteAndRelative) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch, ListDirection::Right);
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

TEST(LayoutTest, layoutListStretchRelativeAndAbsolute) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch, ListDirection::Right);
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

TEST(LayoutTest, layoutListStretchMultiAbsolutAndRelative) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch, ListDirection::Right);
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

TEST(LayoutTest, layoutListStretchDown) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch, ListDirection::Down);
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
    ASSERT_FLOAT_EQ(transform1[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform1[1][2], 25.0f);
    // Child 2 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform2[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform2[1][2], 75.0f);
}

TEST(LayoutTest, layoutListStretchUp) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch, ListDirection::Up);
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
    ASSERT_FLOAT_EQ(transform1[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform1[1][2], 75.0f);
    // Child 2 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform2[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform2[1][2], 25.0f);
}

TEST(LayoutTest, layoutListStretchLeft) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::ListStretch, ListDirection::Left);
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
    ASSERT_FLOAT_EQ(transform1[0][2], 75.0f);
    // Child 2 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform2[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform2[0][2], 25.0f);
}

TEST(LayoutTest, layoutList2Children) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::List);
    Layout child1(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(1.0, 50));
    Layout child2(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(1.0, 50));
    rootLayout.resolveTransform();
    Mat3 transform1 = child1.getTransform();
    Mat3 transform2 = child2.getTransform();
    Mat3 listTransform = listLayout.getTransform();
    std::cout << listTransform << std::endl;
    std::cout << transform1 << std::endl;
    std::cout << transform2 << std::endl;
    // Child 1 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform1[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform1[1][2], 25.0f);
    // Child 2 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform2[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform2[1][2], 75.0f);
}

TEST(LayoutTest, layoutList3Children) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::List);
    Layout child1(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(1.0, 50));
    Layout child2(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(1.0, .3));
    Layout child3(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(1.0, 20));
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
    ASSERT_FLOAT_EQ(transform1[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform1[1][2], 25.0f);
    // Child 2 should be 1/4 of the space
    ASSERT_FLOAT_EQ(transform2[1][1], 30.0f);
    ASSERT_FLOAT_EQ(transform2[1][2], 65.0f);
    // Child 3 should be 1/4 of the space
    ASSERT_FLOAT_EQ(transform3[1][1], 20.0f);
    ASSERT_FLOAT_EQ(transform3[1][2], 90.0f);
}

TEST(LayoutTest, layoutListRight) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::List, ListDirection::Right);
    Layout child1(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 1.0));
    Layout child2(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 1.0));
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

TEST(LayoutTest, layoutListLeft) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::List, ListDirection::Left);
    Layout child1(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 1.0));
    Layout child2(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 1.0));
    rootLayout.resolveTransform();
    Mat3 transform1 = child1.getTransform();
    Mat3 transform2 = child2.getTransform();
    Mat3 listTransform = listLayout.getTransform();
    std::cout << listTransform << std::endl;
    std::cout << transform1 << std::endl;
    std::cout << transform2 << std::endl;
    // Child 1 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform1[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform1[0][2], 75.0f);
    // Child 2 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform2[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform2[0][2], 25.0f);
}

TEST(LayoutTest, layoutListUp) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout listLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(.5, .5), ChildPlacement::List, ListDirection::Up);
    Layout child1(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(1.0, 50));
    Layout child2(&listLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(1.0, 50));
    rootLayout.resolveTransform();
    Mat3 transform1 = child1.getTransform();
    Mat3 transform2 = child2.getTransform();
    Mat3 listTransform = listLayout.getTransform();
    std::cout << listTransform << std::endl;
    std::cout << transform1 << std::endl;
    std::cout << transform2 << std::endl;
    // Child 1 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform1[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform1[1][2], 75.0f);
    // Child 2 should be 1/2 of the space
    ASSERT_FLOAT_EQ(transform2[1][1], 50.0f);
    ASSERT_FLOAT_EQ(transform2[1][2], 25.0f);
}

TEST(LayoutTest, layoutSizingFit) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout parentLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 100), ChildPlacement::Free, ListDirection::Right, Sizing::Fit, Sizing::Fit);
    Layout child(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 50));
    rootLayout.resolveTransform();
    Mat3 transform = parentLayout.getTransform();

    std::cout << transform << std::endl;
    std::cout << child.getTransform() << std::endl;

    ASSERT_FLOAT_EQ(transform[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 50.0f);
}

TEST(LayoutTest, layoutSizingFitWidth) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout parentLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 100), ChildPlacement::Free, ListDirection::Right, Sizing::Fit, Sizing::Fixed);
    Layout child(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 50));
    rootLayout.resolveTransform();
    Mat3 transform = parentLayout.getTransform();

    std::cout << transform << std::endl;
    std::cout << child.getTransform() << std::endl;

    ASSERT_FLOAT_EQ(transform[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
}

TEST(LayoutTest, layoutSizingFitHeight) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout parentLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 100), ChildPlacement::Free, ListDirection::Right, Sizing::Fixed, Sizing::Fit);
    Layout child(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 50));
    rootLayout.resolveTransform();
    Mat3 transform = parentLayout.getTransform();

    std::cout << transform << std::endl;
    std::cout << child.getTransform() << std::endl;

    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 50.0f);
}

TEST(LayoutTest, layoutSizingFitMultipleChildren) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout parentLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 100), ChildPlacement::Free, ListDirection::Right, Sizing::Fit, Sizing::Fit);
    Layout child1(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 50));
    Layout child2(&parentLayout, Anchors::TopLeft, MessureVec2(100, 100), Anchors::TopLeft, MessureVec2(50, 50));
    rootLayout.resolveTransform();
    Mat3 transform = parentLayout.getTransform();

    std::cout << transform << std::endl;

    ASSERT_FLOAT_EQ(transform[0][0], 150.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 150.0f);
}

TEST(LayoutTest, layoutSizingFitChildList) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout parentLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 100), ChildPlacement::List, ListDirection::Down, Sizing::Fit, Sizing::Fit);
    Layout child1(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(60, .25));
    Layout child2(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(60, .25));
    rootLayout.resolveTransform();
    Mat3 transform = parentLayout.getTransform();

    std::cout << transform << std::endl;

    ASSERT_FLOAT_EQ(transform[0][0], 60.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 50.0f);
}

TEST(LayoutTest, layoutSizingFitChildListStretch) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout parentLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 100), ChildPlacement::ListStretch, ListDirection::Down, Sizing::Fit, Sizing::Fit);
    Layout child1(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(60, 25));
    Layout child2(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(60, 25));
    rootLayout.resolveTransform();
    Mat3 transform = parentLayout.getTransform();

    std::cout << transform << std::endl;

    ASSERT_FLOAT_EQ(transform[0][0], 60.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 50.0f);
}

TEST(LayoutTest, layoutSizingExpandSmalerThanParent) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout parentLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 100), ChildPlacement::Free, ListDirection::Right, Sizing::Expand, Sizing::Expand);
    Layout child(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 50));
    rootLayout.resolveTransform();
    Mat3 transform = parentLayout.getTransform();

    std::cout << transform << std::endl;
    std::cout << child.getTransform() << std::endl;

    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
}

TEST(LayoutTest, layoutSizingExpandLargerThanParent) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout parentLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 50), ChildPlacement::Free, ListDirection::Right, Sizing::Expand, Sizing::Expand);
    Layout child(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 100));
    rootLayout.resolveTransform();
    Mat3 transform = parentLayout.getTransform();

    std::cout << transform << std::endl;
    std::cout << child.getTransform() << std::endl;

    ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
}

TEST(LayoutTest, layoutSizingShrinkLargerThanParent) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout parentLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 50), ChildPlacement::Free, ListDirection::Right, Sizing::Shrink, Sizing::Shrink);
    Layout child(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 100));
    rootLayout.resolveTransform();
    Mat3 transform = parentLayout.getTransform();

    std::cout << transform << std::endl;
    std::cout << child.getTransform() << std::endl;

    ASSERT_FLOAT_EQ(transform[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 50.0f);
}

TEST(LayoutTest, layoutSizingShrinkSmalerThanParent) {
    Layout rootLayout(MessureVec2(200, 200));
    Layout parentLayout(&rootLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(100, 100), ChildPlacement::Free, ListDirection::Right, Sizing::Shrink, Sizing::Shrink);
    Layout child(&parentLayout, Anchors::TopLeft, MessureVec2(0, 0), Anchors::TopLeft, MessureVec2(50, 50));
    rootLayout.resolveTransform();
    Mat3 transform = parentLayout.getTransform();

    std::cout << transform << std::endl;
    std::cout << child.getTransform() << std::endl;

    ASSERT_FLOAT_EQ(transform[0][0], 50.0f);
    ASSERT_FLOAT_EQ(transform[1][1], 50.0f);
}

}  // namespace gltk
