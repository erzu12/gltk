#pragma once

#include "layout/relative_scene.h"

namespace gltk {

class LayoutBuilder {
  public:
    LayoutBuilder(RelativeLayout *parent);

    LayoutBuilder &setSize(MessureVec2 size);
    LayoutBuilder &setRenderable(std::unique_ptr<IRenderable> renderable);
    LayoutBuilder &addBox(Style style);
    LayoutBuilder &setOffset(MessureVec2 offset);
    LayoutBuilder &setSizing(Sizing sizing);
    LayoutBuilder &setAnchor(Vec2 anchor);
    LayoutBuilder &setPivot(Vec2 pivot);
    LayoutBuilder &setPadding(Padding padding);
    LayoutBuilder &setChildPlacement(ChildPlacement childPlacement);
    LayoutBuilder &setListDirection(ListDirection listDirection);
    LayoutBuilder &setOverflow(Overflow overflow);
    std::unique_ptr<RelativeLayout> build();

  private:
    bool box = false;
    Style style;
    RelativeLayout *parent;
    std::unique_ptr<IRenderable> renderable = nullptr;
    Vec2 anchor = Anchors::Center;
    MessureVec2 offset = MessureVec2(0_px, 0_px);
    Sizing sizing = {SizingMode::Layout, SizingMode::Layout};
    Vec2 pivot = Anchors::Center;
    Padding padding = {0, 0, 0, 0};
    MessureVec2 size = MessureVec2(0_px, 0_px);
    ChildPlacement childPlacement = ChildPlacement::Free;
    ListDirection listDirection = ListDirection::Down;
    Overflow overflow = Overflow::Scroll;
};

} // namespace gltk
