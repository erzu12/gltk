#pragma once

#include <layout/scene.h>
#include <messure.h>
#include <style.h>
#include <window.h>

namespace gltk {

struct SliderSettings {
    double initialValue = 0.0;
    double minValue = 0.0;
    double maxValue = 100.0;
    double step = 0.0;
    std::shared_ptr<StyleSheet> styleSheet = nullptr;
    MessureVec2 size = MessureVec2(AbsoluteMessure(300), AbsoluteMessure(75));
    int thumbSize = 40;
    Color baseColor = Color(0.3f, 0.3f, 0.3f);
    Color activeColor = Color(0.3f, 0.3f, 0.3f);
};

class Slider {
    SliderSettings settings;
    std::vector<std::function<void(double)>> valueChangeCallbacks;
    Layout *thumbLimit;
    Layout *activeTrack;
    Layout *inactiveTrack;
    Layout *thumb;
    double currentValue = 0.0;
    bool isDragging = false;

    void moveThumb(Vec2 localPos);

  public:
    Slider(Scene *scene, Window *window, Layout *parent, SliderSettings settings = {});

    void registerValueChangeCallback(std::function<void(double)> callback) { valueChangeCallbacks.push_back(callback); }
};

} // namespace gltk
