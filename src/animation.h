#pragma once

#include "color.h"
#include "vec_math.h"
#include <functional>

namespace gltk {

template <typename T>
concept VectorType = std::is_same_v<T, Vec2> || std::is_same_v<T, Vec3> || std::is_same_v<T, Color>;
template <typename T>
concept AnimatableType = std::integral<T> || std::floating_point<T> || VectorType<T>;

template <AnimatableType T> class Animatable {
    T value;
    T targetValue;
    T startValue;
    float duration = 0.0f;
    float elapsedTime = 0.0f;
    std::function<float(float)> easingFunc = nullptr;
    std::function<void(const T &, const T &)> onUpdate = nullptr;

  public:
    void update(float deltaTime) {
        if (elapsedTime < duration) {
            elapsedTime += deltaTime;
            elapsedTime = std::min(elapsedTime, duration);
            T lastValue = value;
            float t = elapsedTime / duration;
            if (easingFunc) {
                t = easingFunc(t);
            }
            value = startValue + (targetValue - startValue) * t;
            if (elapsedTime >= duration) {
                value = targetValue;
            }
            if (onUpdate) {
                onUpdate(value - lastValue, value);
            }
        }
    }

    Animatable() : value{} {}
    Animatable(const T &initialValue) : value(initialValue) {}

    void
    animate(const T &targetValue, float durationSeconds, std::function<void(const T &delta, const T &value)> onUpdate) {
        animate(targetValue, durationSeconds, nullptr, onUpdate);
    }

    void animate(
        const T &targetValue,
        float durationSeconds,
        std::function<float(float)> easing = nullptr,
        std::function<void(const T &delta, const T &value)> onUpdate = nullptr
    ) {
        startValue = value;
        this->targetValue = targetValue;
        duration = durationSeconds;
        elapsedTime = 0.0f;
        easingFunc = easing;
        this->onUpdate = onUpdate;
    }

    void stopAnimation() {
        onUpdate = nullptr;
        duration = 0.0f;
        elapsedTime = 0.0f;
    }

    T get() const { return value; }
    void set(const T &newValue) { value = newValue; }

    Animatable<T> &operator=(const T &newValue) {
        set(newValue);
        return *this;
    }

    operator T() const { return get(); }
};

namespace easing {
float easeOutBounce(float x);
float easeInQuad(float x);
float easeOutQuad(float x);
float easeInOutQuad(float x);
float easeInCubic(float x);
float easeOutCubic(float x);
float easeInOutCubic(float x);
float easeInQuart(float x);
float easeOutQuart(float x);
float easeInOutQuart(float x);
float easeInQuint(float x);
float easeOutQuint(float x);
float easeInOutQuint(float x);
float easeInSine(float x);
float easeOutSine(float x);
float easeInOutSine(float x);
float easeInExpo(float x);
float easeOutExpo(float x);
float easeInOutExpo(float x);
float easeInCirc(float x);
float easeOutCirc(float x);
float easeInOutCirc(float x);
float easeInBack(float x);
float easeOutBack(float x);
float easeInOutBack(float x);
float easeInElastic(float x);
float easeOutElastic(float x);
float easeInOutElastic(float x);
float easeInBounce(float x);
float easeInOutBounce(float x);

} // namespace easing

} // namespace gltk
