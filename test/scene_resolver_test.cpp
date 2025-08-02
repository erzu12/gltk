#include <gtest/gtest.h>
#include "../src/layout/relative_scene.h"
#include "../src/layout/resolved_scene.h"
#include "../src/layout/scene_resolver.h"

namespace gltk {

    class LayoutTest : public ::testing::Test {
    protected:
        void SetUp() override {
            auto rootLayout = std::make_unique<RelativeLayout>();
            rootLayout->positioning = Positioning{.size = MessureVec2(200_px, 200_px), .anchor = Anchors::TopLeft, .pivot = Anchors::TopLeft};
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
        layout->positioning = Positioning{.size = MessureVec2(100_px, 100_px), .anchor = Anchors::Center, .pivot = Anchors::Center};

        const RelativeLayout *layoutRef = scene->addRelativeLayout(std::move(layout), rootLayoutRef);

        auto resolvedScene = resolveScene(*scene, Vec2(200, 200));

        Mat3 transform = resolvedScene->getLayout(layoutRef->id)->Transform;
        ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
        ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
        ASSERT_FLOAT_EQ(transform[0][2], 100.0f);
        ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
    }

    TEST_F(LayoutTest, topLeftSquareAbsolute) {
        auto layout = std::make_unique<RelativeLayout>();
        layout->positioning = Positioning{.anchor = Anchors::TopLeft, .pivot = Anchors::TopLeft};
        layout->positioning.size = MessureVec2(100_px, 100_px);

        const RelativeLayout *layoutRef = scene->addRelativeLayout(std::move(layout), rootLayoutRef);

        auto resolvedScene = resolveScene(*scene, Vec2(200, 200));

        Mat3 transform = resolvedScene->getLayout(layoutRef->id)->Transform;
        ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
        ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
        ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
        ASSERT_FLOAT_EQ(transform[1][2], 50.0f);
    }

    TEST_F(LayoutTest, topLeftSquareRelative) {
        auto layout = std::make_unique<RelativeLayout>();
        layout->positioning = Positioning{.size = MessureVec2(50_pct, 50_pct), .anchor = Anchors::TopLeft, .pivot = Anchors::TopLeft};

        const RelativeLayout *layoutRef = scene->addRelativeLayout(std::move(layout), rootLayoutRef);

        auto resolvedScene = resolveScene(*scene, Vec2(200, 200));

        Mat3 transform = resolvedScene->getLayout(layoutRef->id)->Transform;
        ASSERT_FLOAT_EQ(transform[0][0], 100.0f);
        ASSERT_FLOAT_EQ(transform[1][1], 100.0f);
        ASSERT_FLOAT_EQ(transform[0][2], 50.0f);
        ASSERT_FLOAT_EQ(transform[1][2], 50.0f);
    }

    TEST_F(LayoutTest, centerSquareAbsoluteWithPadding) {
        auto parent = std::make_unique<RelativeLayout>();
        parent->positioning = Positioning{.size = MessureVec2(100_px, 100_px), .anchor = Anchors::Center, .pivot = Anchors::Center, .padding = Padding{10, 10, 10, 10}};
        RelativeLayout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);
        auto layout = std::make_unique<RelativeLayout>();
        layout->positioning = Positioning{.size = MessureVec2(100_pct, 100_pct), .anchor = Anchors::Center, .pivot = Anchors::Center};
        const RelativeLayout *layoutRef = scene->addRelativeLayout(std::move(layout), parentRef);

        auto resolvedScene = resolveScene(*scene, Vec2(200, 200));

        Mat3 transform = resolvedScene->getLayout(layoutRef->id)->Transform;
        ASSERT_FLOAT_EQ(transform[0][0], 80.0f);
        ASSERT_FLOAT_EQ(transform[1][1], 80.0f);
        ASSERT_FLOAT_EQ(transform[0][2], 100.0f);
        ASSERT_FLOAT_EQ(transform[1][2], 100.0f);
    }


}
