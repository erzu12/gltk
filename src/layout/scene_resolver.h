#pragma once

#include "scene.h"
#include "vec_math.h"

namespace gltk {

class LayoutResolver {
    static Vec2 getFinalSize(Vec2 size, Sizing sizing, IRenderable *renderable);
    static void calculateSize(Layout *layout, Vec2 parentSize);
    static void calculatePosition(Layout *layout, Vec2 parentSize, Vec2 parentPosition);
    static void calculateFreeTransforms(const std::vector<Layout *> &children, Vec2 parentSize, Vec2 parentPosition);
    static void calculateListTransforms(
        const std::vector<Layout *> &children, Vec2 parentSize, Vec2 parentPosition, ListDirection listDirection
    );

    static void calculateListStretchTransforms(
        const std::vector<Layout *> &children, Vec2 parentSize, Vec2 parentPosition, ListDirection listDirection
    );

    static void
    resolveLayouts(const std::vector<Layout *> &relativeLayouts, const Layout *parent, const Vec2 viewportSize);
    static void resolveRootLayout(Layout *rootLayout, Vec2 viewportSize);
    static void sendTransformChangeEvents(
        const std::vector<Layout *> &relativeLayouts, const std::vector<Transform> &startTransfomrs
    );

    const static Vec2 ListDireectionVector[4];

    const static int ListDirectionAxis[4];

  public:
    static void resolveScene(Scene &scene, Vec2 viewportSize);
};

} // namespace gltk
