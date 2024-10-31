#pragma once

#include "box.h"
#include "layout.h"

namespace gltk {

class LayoutBuilder {
    public:
        LayoutBuilder(Layout *parent);

        LayoutBuilder& setSize(MessureVec2 size);
        LayoutBuilder& setRenderable(std::unique_ptr<IRenderable> renderable);
        LayoutBuilder& addBox(Style style);
        LayoutBuilder& setOffset(MessureVec2 offset);
        LayoutBuilder& setAnchor(Vec2 anchor);
        LayoutBuilder& setPivot(Vec2 pivot);
        LayoutBuilder& setMargin(Margin margin);
        LayoutBuilder& setPadding(Padding padding);
        LayoutBuilder& setChildPlacement(ChildPlacement childPlacement);
        LayoutBuilder& setListDirection(ListDirection listDirection);
        LayoutBuilder& setSizing(Sizing horizontalSizing, Sizing verticalSizing);
        LayoutBuilder& setOverflow(Overflow overflow);
        std::unique_ptr<Layout> build();

    private:
        bool box = false;
        Style style;
        Layout *parent;
        std::unique_ptr<IRenderable> renderable = nullptr;
        Vec2 anchor = Anchors::TopLeft;
        MessureVec2 offset = MessureVec2(0, 0);
        Vec2 pivot = Vec2(-1.0f);
        Margin margin = {0, 0, 0, 0};
        Padding padding = {0, 0, 0, 0};
        MessureVec2 size = MessureVec2(0, 0);
        ChildPlacement childPlacement = ChildPlacement::Free;
        ListDirection listDirection = ListDirection::Down;
        Sizing horizontalSizing = Sizing::Fixed;
        Sizing verticalSizing = Sizing::Fixed;
        Overflow overflow = Overflow::Scroll;
};


} // namespace gltk
