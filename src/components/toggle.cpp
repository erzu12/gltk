#include "toggle.h"
#include "box.h"
#include "layout_builder.h"

namespace gltk {

ToggleButton::ToggleButton(RelativeScene *scene, RelativeLayout *parent, ToggleButtonSettings settings) {

    auto toggleRoot = LayoutBuilder(scene)
                          .setSize(MessureVec2(AbsoluteMessure(settings.size * 2), AbsoluteMessure(settings.size)))
                          .build();
    auto toggleSlider =
        LayoutBuilder(scene, toggleRoot)
            .setRenderable(
                std::make_unique<Box>(Style{.color = settings.offColor, .radius = static_cast<float>(settings.size)})
            )
            .setSize(MessureVec2(100_pct, 100_pct))
            .build();
    auto toggleDot =
        LayoutBuilder(scene, toggleRoot)
            .setRenderable(
                std::make_unique<Box>(Style{.color = settings.dotColor, .radius = static_cast<float>(settings.size)})
            )
            .setAnchor(Anchors::CenterLeft)
            .setSize(MessureVec2(40_pct, 80_pct))
            .setOffset(MessureVec2(25_pct, 0_px))
            .build();

    scene->addEventCallback<MouseButtonEvent>(
        [=, this](MouseButtonEvent &event) {
            if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::PRESS) {
                Style *style = toggleSlider->renderable.value()->getStyle();
                if (isOn) {
                    scene->addAnimation(
                        toggleDot->positioning.offset.x.get(),
                        75_pct,
                        25_pct,
                        settings.animationDuration,
                        settings.easingFunc
                    );
                    scene->addAnimation(&style->color, settings.onColor, settings.offColor, settings.animationDuration);
                } else {
                    scene->addAnimation(
                        toggleDot->positioning.offset.x.get(),
                        25_pct,
                        75_pct,
                        settings.animationDuration,
                        settings.easingFunc
                    );
                    scene->addAnimation(&style->color, settings.offColor, settings.onColor, settings.animationDuration);
                }
                isOn = !isOn;
                for (const auto &callback : stateChangeCallbacks) {
                    callback(isOn);
                }
            }
        },
        toggleRoot
    );
}

} // namespace gltk
