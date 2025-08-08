#include "../src/layout/relative_scene.h"
#include "../src/layout/resolved_scene.h"
#include "../src/layout/scene_resolver.h"
#include <gtest/gtest.h>

namespace gltk {

class LayoutTest : public ::testing::Test {
  protected:
    void SetUp() override {
        auto rootLayout = std::make_unique<RelativeLayout>();
        rootLayout->positioning =
            Positioning{.size = MessureVec2(200_px, 200_px), .anchor = Anchors::TopLeft, .pivot = Anchors::TopLeft};
        scene = std::make_unique<RelativeScene>();
        rootLayoutRef = scene->addRelativeLayout(std::move(rootLayout));
    }

    void TearDown() override {
        // Code here will be called just after the test as cleanup.
    }
    RelativeLayout *rootLayoutRef = nullptr;
    std::unique_ptr<RelativeScene> scene;
};

TEST_F(LayoutTest, centerSquareAbsolute) {
    auto layout = std::make_unique<RelativeLayout>();
    layout->positioning =
        Positioning{.size = MessureVec2(100_px, 100_px), .anchor = Anchors::Center, .pivot = Anchors::Center};

    const RelativeLayout *layoutRef = scene->addRelativeLayout(std::move(layout), rootLayoutRef);

    auto resolvedScene = resolveScene(*scene, Vec2(200, 200));

    Vec2 position = resolvedScene->getLayout(layoutRef->id)->Position;
    Vec2 size = resolvedScene->getLayout(layoutRef->id)->Size;

    ASSERT_FLOAT_EQ(size.x, 100.0f);
    ASSERT_FLOAT_EQ(size.y, 100.0f);
    ASSERT_FLOAT_EQ(position.x, 100.0f);
    ASSERT_FLOAT_EQ(position.y, 100.0f);
}

TEST_F(LayoutTest, topLeftSquareAbsolute) {
    auto layout = std::make_unique<RelativeLayout>();
    layout->positioning = Positioning{.anchor = Anchors::TopLeft, .pivot = Anchors::TopLeft};
    layout->positioning.size = MessureVec2(100_px, 100_px);

    const RelativeLayout *layoutRef = scene->addRelativeLayout(std::move(layout), rootLayoutRef);

    auto resolvedScene = resolveScene(*scene, Vec2(200, 200));

    Vec2 position = resolvedScene->getLayout(layoutRef->id)->Position;
    Vec2 size = resolvedScene->getLayout(layoutRef->id)->Size;

    ASSERT_FLOAT_EQ(size.x, 100.0f);
    ASSERT_FLOAT_EQ(size.y, 100.0f);
    ASSERT_FLOAT_EQ(position.x, 50.0f);
    ASSERT_FLOAT_EQ(position.y, 50.0f);
}

TEST_F(LayoutTest, differentAnchorAndPivotSquareAbsolute) {
    auto layout = std::make_unique<RelativeLayout>();
    layout->positioning = Positioning{.anchor = Anchors::Center, .pivot = Anchors::TopLeft};
    layout->positioning.size = MessureVec2(100_px, 100_px);

    const RelativeLayout *layoutRef = scene->addRelativeLayout(std::move(layout), rootLayoutRef);

    auto resolvedScene = resolveScene(*scene, Vec2(200, 200));

    Vec2 position = resolvedScene->getLayout(layoutRef->id)->Position;
    Vec2 size = resolvedScene->getLayout(layoutRef->id)->Size;

    ASSERT_FLOAT_EQ(size.x, 100.0f);
    ASSERT_FLOAT_EQ(size.y, 100.0f);
    ASSERT_FLOAT_EQ(position.x, 150.0f);
    ASSERT_FLOAT_EQ(position.y, 150.0f);
}

TEST_F(LayoutTest, topLeftSquareRelative) {
    auto layout = std::make_unique<RelativeLayout>();
    layout->positioning =
        Positioning{.size = MessureVec2(50_pct, 50_pct), .anchor = Anchors::TopLeft, .pivot = Anchors::TopLeft};

    const RelativeLayout *layoutRef = scene->addRelativeLayout(std::move(layout), rootLayoutRef);

    auto resolvedScene = resolveScene(*scene, Vec2(200, 200));

    Vec2 position = resolvedScene->getLayout(layoutRef->id)->Position;
    Vec2 size = resolvedScene->getLayout(layoutRef->id)->Size;

    ASSERT_FLOAT_EQ(size.x, 100.0f);
    ASSERT_FLOAT_EQ(size.y, 100.0f);
    ASSERT_FLOAT_EQ(position.x, 50.0f);
    ASSERT_FLOAT_EQ(position.y, 50.0f);
}

TEST_F(LayoutTest, centerSquareAbsoluteWithPadding) {
    auto parent = std::make_unique<RelativeLayout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::Center,
        .pivot = Anchors::Center,
        .padding = Padding{10, 10, 10, 10}
    };
    RelativeLayout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);
    auto layout = std::make_unique<RelativeLayout>();
    layout->positioning =
        Positioning{.size = MessureVec2(100_pct, 100_pct), .anchor = Anchors::Center, .pivot = Anchors::Center};
    const RelativeLayout *layoutRef = scene->addRelativeLayout(std::move(layout), parentRef);

    auto resolvedScene = resolveScene(*scene, Vec2(200, 200));

    Vec2 position = resolvedScene->getLayout(layoutRef->id)->Position;
    Vec2 size = resolvedScene->getLayout(layoutRef->id)->Size;

    ASSERT_FLOAT_EQ(size.x, 80.0f);
    ASSERT_FLOAT_EQ(size.y, 80.0f);
    ASSERT_FLOAT_EQ(position.x, 100.0f);
    ASSERT_FLOAT_EQ(position.y, 100.0f);
}

TEST_F(LayoutTest, centerSquareAbsoluteWithTopLeftPadding) {
    auto parent = std::make_unique<RelativeLayout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::Center,
        .pivot = Anchors::Center,
        .padding = Padding{20, 0, 0, 20}
    };
    RelativeLayout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);
    auto layout = std::make_unique<RelativeLayout>();
    layout->positioning =
        Positioning{.size = MessureVec2(100_pct, 100_pct), .anchor = Anchors::Center, .pivot = Anchors::Center};
    const RelativeLayout *layoutRef = scene->addRelativeLayout(std::move(layout), parentRef);

    auto resolvedScene = resolveScene(*scene, Vec2(200, 200));

    Vec2 position = resolvedScene->getLayout(layoutRef->id)->Position;
    Vec2 size = resolvedScene->getLayout(layoutRef->id)->Size;

    ASSERT_FLOAT_EQ(size.x, 80.0f);
    ASSERT_FLOAT_EQ(size.y, 80.0f);
    ASSERT_FLOAT_EQ(position.x, 110.0f);
    ASSERT_FLOAT_EQ(position.y, 110.0f);
}

TEST_F(LayoutTest, centerSquareAbsoluteWithRandomPadding) {
    auto parent = std::make_unique<RelativeLayout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::Center,
        .pivot = Anchors::Center,
        .padding = Padding{30, 30, 10, 10}
    };
    RelativeLayout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);
    auto layout = std::make_unique<RelativeLayout>();
    layout->positioning =
        Positioning{.size = MessureVec2(100_pct, 100_pct), .anchor = Anchors::Center, .pivot = Anchors::Center};
    const RelativeLayout *layoutRef = scene->addRelativeLayout(std::move(layout), parentRef);

    auto resolvedScene = resolveScene(*scene, Vec2(200, 200));

    Vec2 position = resolvedScene->getLayout(layoutRef->id)->Position;
    Vec2 size = resolvedScene->getLayout(layoutRef->id)->Size;

    ASSERT_FLOAT_EQ(size.x, 60.0f);
    ASSERT_FLOAT_EQ(size.y, 60.0f);
    ASSERT_FLOAT_EQ(position.x, 90.0f);
    ASSERT_FLOAT_EQ(position.y, 110.0f);
}

} // namespace gltk
