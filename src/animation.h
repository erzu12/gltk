#pragma once

#include "color.h"
#include "messure.h"
#include <functional>

namespace gltk {

class IAnimationRunner {
  public:
    virtual void update(float deltaTime) = 0;
    virtual bool isCompleted() const = 0;
    virtual ~IAnimationRunner() = default;
};

template <typename T>
concept VectorType = std::is_same_v<T, Vec2> || std::is_same_v<T, Vec3> || std::is_same_v<T, Color>;
template <typename T>
concept AnimatableType =
    std::integral<T> || std::floating_point<T> || std::derived_from<T, MessureVec2> || VectorType<T>;

template <AnimatableType T> class AnimationRunner : public IAnimationRunner {
  private:
    T *property;
    T startValue;
    T endValue;
    float duration;
    float elapsedTime = 0.0f;
    std::function<float(float)> easingFunc = nullptr;

    void update(float deltaTime) {
        elapsedTime += deltaTime;
        if (elapsedTime >= duration) {
            *property = endValue;
            elapsedTime = duration; // Clamp to duration
        } else {
            float progress = elapsedTime / duration;
            if (easingFunc) {
                progress = easingFunc(progress);
            }
            *property = startValue + (endValue - startValue) * progress;
        }
    }

    bool isCompleted() const { return elapsedTime >= duration; }

  public:
    AnimationRunner(T *property, T endValue, float duration, std::function<float(float)> easingFunc = nullptr)
        : property(property), startValue(*property), endValue(endValue), duration(duration), easingFunc(easingFunc) {}
};

template <typename T>
    requires std::derived_from<T, MessureVec2>
class AnimationRunner<T> : public IAnimationRunner {
  private:
    MessureVec2 *property;
    Vec2 startValue;
    MessureVec2 endValue;
    float duration;
    float elapsedTime = 0.0f;
    std::function<float(float)> easingFunc = nullptr;

    void update(float deltaTime) {
        elapsedTime += deltaTime;
        if (elapsedTime >= duration) {
            property->x->setValue(endValue.x->getValue());
            property->y->setValue(endValue.y->getValue());
            elapsedTime = duration; // Clamp to duration
        } else {
            float progress = elapsedTime / duration;
            if (easingFunc) {
                progress = easingFunc(progress);
            }
            float newValueX = startValue.x + (endValue.x->getValue() - startValue.x) * progress;
            float newValueY = startValue.y + (endValue.y->getValue() - startValue.y) * progress;
            property->x->setValue(newValueX);
            property->y->setValue(newValueY);
        }
    }

    bool isCompleted() const { return elapsedTime >= duration; }

  public:
    AnimationRunner(
        MessureVec2 *property, MessureVec2 &endValue, float duration, std::function<float(float)> easingFunc = nullptr
    )
        : property(property), startValue(property->x->getValue(), property->y->getValue()),
          endValue(std::move(endValue)), duration(duration), easingFunc(easingFunc) {}
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
