#pragma once

#include "layout/scene.h"

namespace gltk {

class LayoutBuilder {
  public:
    LayoutBuilder(Scene *scene);
    LayoutBuilder(Scene *scene, Layout *parent);

    LayoutBuilder &setSize(MessureVec2 size);
    LayoutBuilder &setRenderable(std::unique_ptr<IRenderable> renderable);
    LayoutBuilder &setOffset(MessureVec2 offset);
    LayoutBuilder &setSizing(Sizing sizing);
    LayoutBuilder &setAnchor(Vec2 anchor);
    LayoutBuilder &setPivot(Vec2 pivot);
    LayoutBuilder &setPadding(Padding padding);
    LayoutBuilder &setChildPlacement(ChildPlacement childPlacement);
    LayoutBuilder &setListDirection(ListDirection listDirection);
    LayoutBuilder &setClipOverflow(bool overflow);
    LayoutBuilder &setZIndex(int zIndex);
    LayoutBuilder &setVisible(bool visible);
    Layout *build();

  private:
    Scene *scene;
    Layout *parent;
    std::optional<std::unique_ptr<IRenderable>> renderable = std::nullopt;
    Vec2 anchor = Anchors::Center;
    Vec2 pivot = Vec2(-1, -1);
    MessureVec2 offset = MessureVec2(0_px, 0_px);
    Sizing sizing = {SizingMode::Layout, SizingMode::Layout};
    Padding padding = {0, 0, 0, 0};
    MessureVec2 size = MessureVec2(0_px, 0_px);
    ChildPlacement childPlacement = ChildPlacement::Free;
    ListDirection listDirection = ListDirection::Down;
    bool clipOverflow = true;
    bool visible = true;
    int zIndex = 0;
};

} // namespace gltk
