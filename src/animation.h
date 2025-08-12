#pragma once

#include "color.h"
#include "messure.h"
#include <algorithm>
#include <chrono>
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
concept AnimatableType = std::integral<T> || std::floating_point<T> || std::derived_from<T, IMessure> || VectorType<T>;

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
    AnimationRunner(
        T *property, T startValue, T endValue, float duration, std::function<float(float)> easingFunc = nullptr
    )
        : property(property), startValue(startValue), endValue(endValue), duration(duration), easingFunc(easingFunc) {}
};

template <typename T>
    requires std::derived_from<T, IMessure>
class AnimationRunner<T> : public IAnimationRunner {
  private:
    IMessure *property;
    std::unique_ptr<IMessure> startValue;
    std::unique_ptr<IMessure> endValue;
    float duration;
    float elapsedTime = 0.0f;
    std::function<float(float)> easingFunc = nullptr;

    void update(float deltaTime) {
        elapsedTime += deltaTime;
        if (elapsedTime >= duration) {
            property->setValue(endValue->getValue());
            elapsedTime = duration; // Clamp to duration
        } else {
            float progress = elapsedTime / duration;
            if (easingFunc) {
                progress = easingFunc(progress);
            }
            float newValue = startValue->getValue() + (endValue->getValue() - startValue->getValue()) * progress;
            property->setValue(newValue);
        }
    }

    bool isCompleted() const { return elapsedTime >= duration; }

  public:
    AnimationRunner(
        IMessure *property,
        std::unique_ptr<IMessure> startValue,
        std::unique_ptr<IMessure> endValue,
        float duration,
        std::function<float(float)> easingFunc = nullptr
    )
        : property(property), startValue(std::move(startValue)), endValue(std::move(endValue)), duration(duration),
          easingFunc(easingFunc) {}
};

class AnimationManager {
  private:
    std::vector<std::unique_ptr<IAnimationRunner>> animationRunners;
    std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;

  public:
    AnimationManager() : lastUpdateTime(std::chrono::steady_clock::now()) {}

    template <typename T>
        requires std::integral<T> || std::floating_point<T> || VectorType<T>
    void
    create(T *property, T startValue, T endValue, float duration, std::function<float(float)> easingFunc = nullptr) {
        animationRunners.push_back(
            std::make_unique<AnimationRunner<T>>(property, startValue, endValue, duration, easingFunc)
        );
    }

    template <typename T>
        requires std::derived_from<T, IMessure>
    void create(
        IMessure *property,
        const T &startValue,
        const T &endValue,
        float duration,
        std::function<float(float)> easingFunc = nullptr
    ) {
        std::unique_ptr<IMessure> startValuePtr = std::make_unique<T>(startValue);
        std::unique_ptr<IMessure> endValuePtr = std::make_unique<T>(endValue);
        auto animRunner = std::make_unique<AnimationRunner<T>>(
            property, std::move(startValuePtr), std::move(endValuePtr), duration, easingFunc
        );
        animationRunners.push_back(std::move(animRunner));
    }

    void update() {
        float deltaTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - lastUpdateTime).count();
        lastUpdateTime = std::chrono::steady_clock::now();
        for (auto &runner : animationRunners) {
            runner->update(deltaTime);
        }
        // Remove completed animations
        animationRunners.erase(
            std::remove_if(
                animationRunners.begin(),
                animationRunners.end(),
                [](const std::unique_ptr<IAnimationRunner> &runner) { return runner->isCompleted(); }
            ),
            animationRunners.end()
        );
    }
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
