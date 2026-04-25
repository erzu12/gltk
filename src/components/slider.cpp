#include "slider.h"
#include <layout_builder.h>
#include <renderables/box.h>

namespace gltk {

Slider::Slider(Scene *scene, Window *window, Layout *parent, SliderSettings settings) {
    this->settings = settings;

    if (settings.styleSheet != nullptr) {
        settings.baseColor = settings.styleSheet->inputBackground().color;
        settings.activeColor = settings.styleSheet->accentBackground().color;
    }
    Style trackStyle = Style({.color = settings.baseColor, .radius = 1000.0f});
    currentValue = std::clamp(settings.initialValue, settings.minValue, settings.maxValue);
    if (settings.step > 0) {
        currentValue = std::round(currentValue / settings.step) * settings.step;
    }

    float pos = (currentValue - settings.minValue) / (settings.maxValue - settings.minValue);
    auto base = LayoutBuilder(scene, parent).setSize(settings.size).build();
    auto trackLimit = LayoutBuilder(scene, base)
                          .setSize(MessureVec2(100_pct, AbsoluteMessure(settings.thumbSize / 2)))
                          .setAnchor(Anchors::Center)
                          .setPadding({
                              0,
                              settings.thumbSize / 4,
                              0,
                              settings.thumbSize / 4,
                          })
                          .build();
    inactiveTrack = LayoutBuilder(scene, trackLimit)
                        .setRenderable(std::make_unique<Box>(trackStyle))
                        .setAnchor(Anchors::CenterRight)
                        .setSize(MessureVec2(RelativeMessure(1.0f - pos), AbsoluteMessure(settings.thumbSize / 2)))
                        .build();
    trackStyle.color = settings.activeColor;
    activeTrack = LayoutBuilder(scene, trackLimit)
                      .setRenderable(std::make_unique<Box>(trackStyle))
                      .setAnchor(Anchors::CenterLeft)
                      .setSize(MessureVec2(RelativeMessure(pos), AbsoluteMessure(settings.thumbSize / 2)))
                      .build();
    AbsoluteMessure thumbSize(settings.thumbSize);
    thumbLimit = LayoutBuilder(scene, base)
                     .setSize(MessureVec2(100_pct, 100_pct))
                     .setAnchor(Anchors::Center)
                     .setPadding({
                         0,
                         settings.thumbSize / 2,
                         0,
                         settings.thumbSize / 2,
                     })
                     .build();
    thumb = LayoutBuilder(scene, thumbLimit)
                .setRenderable(std::make_unique<Box>(Style({.color = settings.activeColor, .radius = 1000.0f})))
                .setAnchor(Anchors::CenterLeft)
                .setPivot(Anchors::Center)
                .setSize(MessureVec2(thumbSize, thumbSize))
                .setOffset(MessureVec2(RelativeMessure(pos), 0_px))
                .build();

    thumbLimit->addEventCallback<MouseButtonEvent>([=, this](MouseButtonEvent &event) {
        if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::PRESS) {
            isDragging = true;
            moveThumb(event.localPos);
        }
    });
    window->add_mouse_up_callback([=, this](MouseButtonEvent event) {
        if (event.button == MouseButton::MOUSE_BUTTON_LEFT && event.action == MouseAction::RELEASE) {
            isDragging = false;
        }
    });
    window->add_mouse_move_callback([=, this](MouseMoveEvent event) {
        if (isDragging) {
            Vec2 localPos = event.pos - thumbLimit->getTransform().Position + thumbLimit->getTransform().Size / 2.0f;
            moveThumb(localPos);
        }
    });
}

void Slider::moveThumb(Vec2 localPos) {
    double newValue = (localPos.x - settings.thumbSize / 2.0) /
                          (thumbLimit->getTransform().Size.x - settings.thumbSize) *
                          (settings.maxValue - settings.minValue) +
                      settings.minValue;
    currentValue = std::clamp(newValue, settings.minValue, settings.maxValue);
    if (settings.step > 0) {
        currentValue = std::round(currentValue / settings.step) * settings.step;
    }
    float pos = (currentValue - settings.minValue) / (settings.maxValue - settings.minValue);
    thumb->getPositioning().offset.x = std::make_shared<RelativeMessure>(pos);
    inactiveTrack->getPositioning().size.x = std::make_shared<RelativeMessure>(1.0f - pos);
    activeTrack->getPositioning().size.x = std::make_shared<RelativeMessure>(pos);
    for (const auto &callback : valueChangeCallbacks) {
        callback(currentValue);
    }
}

} // namespace gltk
