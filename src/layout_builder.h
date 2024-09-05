#pragma once

#include "layout.h"

namespace gltk {

class LayoutBuilder {
    public:
        LayoutBuilder(Layout *parent);

        LayoutBuilder& setSize(MessureVec2 size);
        LayoutBuilder& setRenderable(std::unique_ptr<IRenderable> renderable);
        LayoutBuilder& setOffset(MessureVec2 offset);
        LayoutBuilder& setAnchor(Vec2 anchor);
        LayoutBuilder& setPivot(Vec2 pivot);
        LayoutBuilder& setChildPlacement(ChildPlacement childPlacement);
        LayoutBuilder& setListDirection(ListDirection listDirection);
        LayoutBuilder& setSizing(Sizing horizontalSizing, Sizing verticalSizing);
        LayoutBuilder& setOverflow(Overflow overflow);
        std::unique_ptr<Layout> build();

    private:
        Layout *parent;

        bool pivotSet = false;

        std::unique_ptr<IRenderable> renderable = nullptr;
        Vec2 anchor = Anchors::TopLeft;
        MessureVec2 offset = MessureVec2(0, 0);
        Vec2 pivot = Anchors::TopLeft;
        MessureVec2 size = MessureVec2(0, 0);
        ChildPlacement childPlacement = ChildPlacement::Free;
        ListDirection listDirection = ListDirection::Down;
        Sizing horizontalSizing = Sizing::Fixed;
        Sizing verticalSizing = Sizing::Fixed;
        Overflow overflow = Overflow::None;
};

} // namespace gltk
