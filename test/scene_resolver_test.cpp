#include "../src/layout/scene.h"
#include "../src/layout/scene_resolver.h"
#include <gtest/gtest.h>

namespace gltk {

class LayoutTest : public ::testing::Test {
  protected:
    void SetUp() override {
        auto rootLayout = std::make_unique<Layout>();
        rootLayout->positioning =
            Positioning{.size = MessureVec2(200_px, 200_px), .anchor = Anchors::TopLeft, .pivot = Anchors::TopLeft};
        scene = std::make_unique<Scene>();
        rootLayoutRef = scene->addRelativeLayout(std::move(rootLayout));
    }

    void TearDown() override {
        // Code here will be called just after the test as cleanup.
    }
    Layout *rootLayoutRef = nullptr;
    std::unique_ptr<Scene> scene;
};

TEST_F(LayoutTest, centerSquareAbsolute) {
    auto layout = std::make_unique<Layout>();
    layout->positioning =
        Positioning{.size = MessureVec2(100_px, 100_px), .anchor = Anchors::Center, .pivot = Anchors::Center};

    const Layout *layoutRef = scene->addRelativeLayout(std::move(layout), rootLayoutRef);

    resolveScene(*scene, Vec2(200, 200));

    Vec2 position = scene->getLayout(layoutRef->id)->transform.Position;
    Vec2 size = scene->getLayout(layoutRef->id)->transform.Size;

    ASSERT_FLOAT_EQ(size.x, 100.0f);
    ASSERT_FLOAT_EQ(size.y, 100.0f);
    ASSERT_FLOAT_EQ(position.x, 100.0f);
    ASSERT_FLOAT_EQ(position.y, 100.0f);
}

TEST_F(LayoutTest, topLeftSquareAbsolute) {
    auto layout = std::make_unique<Layout>();
    layout->positioning = Positioning{.anchor = Anchors::TopLeft, .pivot = Anchors::TopLeft};
    layout->positioning.size = MessureVec2(100_px, 100_px);

    const Layout *layoutRef = scene->addRelativeLayout(std::move(layout), rootLayoutRef);

    resolveScene(*scene, Vec2(200, 200));

    Vec2 position = scene->getLayout(layoutRef->id)->transform.Position;
    Vec2 size = scene->getLayout(layoutRef->id)->transform.Size;

    ASSERT_FLOAT_EQ(size.x, 100.0f);
    ASSERT_FLOAT_EQ(size.y, 100.0f);
    ASSERT_FLOAT_EQ(position.x, 50.0f);
    ASSERT_FLOAT_EQ(position.y, 50.0f);
}

TEST_F(LayoutTest, differentAnchorAndPivotSquareAbsolute) {
    auto layout = std::make_unique<Layout>();
    layout->positioning = Positioning{.anchor = Anchors::Center, .pivot = Anchors::TopLeft};
    layout->positioning.size = MessureVec2(100_px, 100_px);

    const Layout *layoutRef = scene->addRelativeLayout(std::move(layout), rootLayoutRef);

    resolveScene(*scene, Vec2(200, 200));

    Vec2 position = scene->getLayout(layoutRef->id)->transform.Position;
    Vec2 size = scene->getLayout(layoutRef->id)->transform.Size;

    ASSERT_FLOAT_EQ(size.x, 100.0f);
    ASSERT_FLOAT_EQ(size.y, 100.0f);
    ASSERT_FLOAT_EQ(position.x, 150.0f);
    ASSERT_FLOAT_EQ(position.y, 150.0f);
}

TEST_F(LayoutTest, topLeftSquareRelative) {
    auto layout = std::make_unique<Layout>();
    layout->positioning =
        Positioning{.size = MessureVec2(50_pct, 50_pct), .anchor = Anchors::TopLeft, .pivot = Anchors::TopLeft};

    const Layout *layoutRef = scene->addRelativeLayout(std::move(layout), rootLayoutRef);

    resolveScene(*scene, Vec2(200, 200));

    Vec2 position = scene->getLayout(layoutRef->id)->transform.Position;
    Vec2 size = scene->getLayout(layoutRef->id)->transform.Size;

    ASSERT_FLOAT_EQ(size.x, 100.0f);
    ASSERT_FLOAT_EQ(size.y, 100.0f);
    ASSERT_FLOAT_EQ(position.x, 50.0f);
    ASSERT_FLOAT_EQ(position.y, 50.0f);
}

TEST_F(LayoutTest, centerSquareAbsoluteWithPadding) {
    auto parent = std::make_unique<Layout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::Center,
        .pivot = Anchors::Center,
        .padding = Padding{10, 10, 10, 10}
    };
    Layout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);
    auto layout = std::make_unique<Layout>();
    layout->positioning =
        Positioning{.size = MessureVec2(100_pct, 100_pct), .anchor = Anchors::Center, .pivot = Anchors::Center};
    const Layout *layoutRef = scene->addRelativeLayout(std::move(layout), parentRef);

    resolveScene(*scene, Vec2(200, 200));

    Vec2 position = scene->getLayout(layoutRef->id)->transform.Position;
    Vec2 size = scene->getLayout(layoutRef->id)->transform.Size;

    ASSERT_FLOAT_EQ(size.x, 80.0f);
    ASSERT_FLOAT_EQ(size.y, 80.0f);
    ASSERT_FLOAT_EQ(position.x, 100.0f);
    ASSERT_FLOAT_EQ(position.y, 100.0f);
}

TEST_F(LayoutTest, centerSquareAbsoluteWithTopLeftPadding) {
    auto parent = std::make_unique<Layout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::Center,
        .pivot = Anchors::Center,
        .padding = Padding{20, 0, 0, 20}
    };
    Layout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);
    auto layout = std::make_unique<Layout>();
    layout->positioning =
        Positioning{.size = MessureVec2(100_pct, 100_pct), .anchor = Anchors::Center, .pivot = Anchors::Center};
    const Layout *layoutRef = scene->addRelativeLayout(std::move(layout), parentRef);

    resolveScene(*scene, Vec2(200, 200));

    Vec2 position = scene->getLayout(layoutRef->id)->transform.Position;
    Vec2 size = scene->getLayout(layoutRef->id)->transform.Size;

    ASSERT_FLOAT_EQ(size.x, 80.0f);
    ASSERT_FLOAT_EQ(size.y, 80.0f);
    ASSERT_FLOAT_EQ(position.x, 110.0f);
    ASSERT_FLOAT_EQ(position.y, 110.0f);
}

TEST_F(LayoutTest, centerSquareAbsoluteWithRandomPadding) {
    auto parent = std::make_unique<Layout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::Center,
        .pivot = Anchors::Center,
        .padding = Padding{30, 30, 10, 10}
    };
    Layout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);
    auto layout = std::make_unique<Layout>();
    layout->positioning =
        Positioning{.size = MessureVec2(100_pct, 100_pct), .anchor = Anchors::Center, .pivot = Anchors::Center};
    const Layout *layoutRef = scene->addRelativeLayout(std::move(layout), parentRef);

    resolveScene(*scene, Vec2(200, 200));

    Vec2 position = scene->getLayout(layoutRef->id)->transform.Position;
    Vec2 size = scene->getLayout(layoutRef->id)->transform.Size;

    ASSERT_FLOAT_EQ(size.x, 60.0f);
    ASSERT_FLOAT_EQ(size.y, 60.0f);
    ASSERT_FLOAT_EQ(position.x, 90.0f);
    ASSERT_FLOAT_EQ(position.y, 110.0f);
}

TEST_F(LayoutTest, listDownAbsolute) {
    auto parent = std::make_unique<Layout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::TopLeft,
        .pivot = Anchors::TopLeft,
        .childPlacement = ChildPlacement::List,
        .listDirection = ListDirection::Down
    };
    Layout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);

    std::vector<Layout *> children;
    for (int i = 0; i < 3; ++i) {
        auto layout = std::make_unique<Layout>();
        layout->positioning =
            Positioning{.size = MessureVec2(100_px, 20_px), .anchor = Anchors::Center, .pivot = Anchors::Center};
        children.push_back(scene->addRelativeLayout(std::move(layout), parentRef));
    }

    resolveScene(*scene, Vec2(200, 200));

    for (size_t i = 0; i < children.size(); ++i) {
        Vec2 position = scene->getLayout(children[i]->id)->transform.Position;
        Vec2 size = scene->getLayout(children[i]->id)->transform.Size;

        ASSERT_FLOAT_EQ(size.x, 100.0f);
        ASSERT_FLOAT_EQ(size.y, 20.0f);
        ASSERT_FLOAT_EQ(position.x, 50.0f);
        ASSERT_FLOAT_EQ(position.y, 10.0f + i * 20.0f);
    }
}

TEST_F(LayoutTest, listLeftAbsolute) {
    auto parent = std::make_unique<Layout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::TopLeft,
        .pivot = Anchors::TopLeft,
        .childPlacement = ChildPlacement::List,
        .listDirection = ListDirection::Left
    };
    Layout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);

    std::vector<Layout *> children;
    for (int i = 0; i < 3; ++i) {
        auto layout = std::make_unique<Layout>();
        layout->positioning =
            Positioning{.size = MessureVec2(20_px, 100_px), .anchor = Anchors::Center, .pivot = Anchors::Center};
        children.push_back(scene->addRelativeLayout(std::move(layout), parentRef));
    }

    resolveScene(*scene, Vec2(200, 200));

    for (size_t i = 0; i < children.size(); ++i) {
        Vec2 position = scene->getLayout(children[i]->id)->transform.Position;
        Vec2 size = scene->getLayout(children[i]->id)->transform.Size;

        ASSERT_FLOAT_EQ(size.x, 20.0f);
        ASSERT_FLOAT_EQ(size.y, 100.0f);
        ASSERT_FLOAT_EQ(position.x, 90.0f - i * 20.0f);
        ASSERT_FLOAT_EQ(position.y, 50.0f);
    }
}

TEST_F(LayoutTest, listDownAbsoluteAnchorTopLeft) {
    auto parent = std::make_unique<Layout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::TopLeft,
        .pivot = Anchors::TopLeft,
        .childPlacement = ChildPlacement::List,
        .listDirection = ListDirection::Down
    };
    Layout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);

    std::vector<Layout *> children;
    for (int i = 0; i < 3; ++i) {
        auto layout = std::make_unique<Layout>();
        layout->positioning =
            Positioning{.size = MessureVec2(100_px, 20_px), .anchor = Anchors::TopLeft, .pivot = Anchors::TopLeft};
        children.push_back(scene->addRelativeLayout(std::move(layout), parentRef));
    }

    resolveScene(*scene, Vec2(200, 200));

    for (size_t i = 0; i < children.size(); ++i) {
        Vec2 position = scene->getLayout(children[i]->id)->transform.Position;
        Vec2 size = scene->getLayout(children[i]->id)->transform.Size;

        ASSERT_FLOAT_EQ(size.x, 100.0f);
        ASSERT_FLOAT_EQ(size.y, 20.0f);
        ASSERT_FLOAT_EQ(position.x, 50.0f);
        ASSERT_FLOAT_EQ(position.y, 10.0f + i * 20.0f);
    }
}

TEST_F(LayoutTest, listDownRelative) {
    auto parent = std::make_unique<Layout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::TopLeft,
        .pivot = Anchors::TopLeft,
        .childPlacement = ChildPlacement::List,
        .listDirection = ListDirection::Down
    };
    Layout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);

    std::vector<Layout *> children;
    for (int i = 0; i < 3; ++i) {
        auto layout = std::make_unique<Layout>();
        layout->positioning =
            Positioning{.size = MessureVec2(100_pct, 20_pct), .anchor = Anchors::Center, .pivot = Anchors::Center};
        children.push_back(scene->addRelativeLayout(std::move(layout), parentRef));
    }

    resolveScene(*scene, Vec2(200, 200));

    for (size_t i = 0; i < children.size(); ++i) {
        Vec2 position = scene->getLayout(children[i]->id)->transform.Position;
        Vec2 size = scene->getLayout(children[i]->id)->transform.Size;

        ASSERT_FLOAT_EQ(size.x, 100.0f);
        ASSERT_FLOAT_EQ(size.y, 20.0f);
        ASSERT_FLOAT_EQ(position.x, 50.0f);
        ASSERT_FLOAT_EQ(position.y, 10.0f + i * 20.0f);
    }
}

TEST_F(LayoutTest, listStretchDownRelative) {
    auto parent = std::make_unique<Layout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::TopLeft,
        .pivot = Anchors::TopLeft,
        .childPlacement = ChildPlacement::ListStretch,
        .listDirection = ListDirection::Down
    };
    Layout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);

    std::vector<Layout *> children;
    for (int i = 0; i < 3; ++i) {
        auto layout = std::make_unique<Layout>();
        layout->positioning =
            Positioning{.size = MessureVec2(100_pct, 100_pct), .anchor = Anchors::Center, .pivot = Anchors::Center};
        children.push_back(scene->addRelativeLayout(std::move(layout), parentRef));
    }

    resolveScene(*scene, Vec2(200, 200));

    for (size_t i = 0; i < children.size(); ++i) {
        Vec2 position = scene->getLayout(children[i]->id)->transform.Position;
        Vec2 size = scene->getLayout(children[i]->id)->transform.Size;

        ASSERT_FLOAT_EQ(size.x, 100.0f);
        ASSERT_FLOAT_EQ(size.y, 100.0f / 3.0f);
        ASSERT_FLOAT_EQ(position.x, 50.0f);
        ASSERT_FLOAT_EQ(position.y, 100.0f / 6.0f + i * 100.0f / 3.0f);
    }
}

TEST_F(LayoutTest, listStretchDownAbsoluteWithAbsolute) {
    auto parent = std::make_unique<Layout>();
    parent->positioning = Positioning{
        .size = MessureVec2(100_px, 100_px),
        .anchor = Anchors::TopLeft,
        .pivot = Anchors::TopLeft,
        .childPlacement = ChildPlacement::ListStretch,
        .listDirection = ListDirection::Down
    };
    Layout *parentRef = scene->addRelativeLayout(std::move(parent), rootLayoutRef);

    auto absolutLayout = std::make_unique<Layout>();
    absolutLayout->positioning =
        Positioning{.size = MessureVec2(100_pct, 25_px), .anchor = Anchors::Center, .pivot = Anchors::Center};
    scene->addRelativeLayout(std::move(absolutLayout), parentRef);

    std::vector<Layout *> children;
    for (int i = 0; i < 3; ++i) {
        auto layout = std::make_unique<Layout>();
        layout->positioning =
            Positioning{.size = MessureVec2(100_pct, 100_pct), .anchor = Anchors::Center, .pivot = Anchors::Center};
        children.push_back(scene->addRelativeLayout(std::move(layout), parentRef));
    }

    resolveScene(*scene, Vec2(200, 200));

    for (size_t i = 0; i < children.size(); ++i) {
        Vec2 position = scene->getLayout(children[i]->id)->transform.Position;
        Vec2 size = scene->getLayout(children[i]->id)->transform.Size;

        ASSERT_FLOAT_EQ(size.x, 100.0f);
        ASSERT_FLOAT_EQ(size.y, 25.0f);
        ASSERT_FLOAT_EQ(position.x, 50.0f);
        ASSERT_FLOAT_EQ(position.y, 25.0f + 12.5f + i * 25.0f);
    }
}

} // namespace gltk
