#pragma once

#include "color.h"
#include "messure.h"
#include <algorithm>
#include <chrono>

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

    void update(float deltaTime) {
        elapsedTime += deltaTime;
        if (elapsedTime >= duration) {
            *property = endValue;
            elapsedTime = duration; // Clamp to duration
        } else {
            float progress = elapsedTime / duration;
            *property = startValue + (endValue - startValue) * progress;
        }
    }

    bool isCompleted() const { return elapsedTime >= duration; }

  public:
    AnimationRunner(T *property, T startValue, T endValue, float duration)
        : property(property), startValue(startValue), endValue(endValue), duration(duration) {}
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

    void update(float deltaTime) {
        elapsedTime += deltaTime;
        if (elapsedTime >= duration) {
            property->setValue(endValue->getValue());
            elapsedTime = duration; // Clamp to duration
        } else {
            float progress = elapsedTime / duration;
            float newValue = startValue->getValue() + (endValue->getValue() - startValue->getValue()) * progress;
            property->setValue(newValue);
        }
    }

    bool isCompleted() const { return elapsedTime >= duration; }

  public:
    AnimationRunner(
        IMessure *property, std::unique_ptr<IMessure> startValue, std::unique_ptr<IMessure> endValue, float duration
    )
        : property(property), startValue(std::move(startValue)), endValue(std::move(endValue)), duration(duration) {}
};

class AnimationManager {
  private:
    std::vector<std::unique_ptr<IAnimationRunner>> animationRunners;
    std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;

  public:
    AnimationManager() : lastUpdateTime(std::chrono::steady_clock::now()) {}

    template <typename T>
        requires std::integral<T> || std::floating_point<T> || VectorType<T>
    void create(T *property, T startValue, T endValue, float duration) {
        animationRunners.push_back(std::make_unique<AnimationRunner<T>>(property, startValue, endValue, duration));
    }

    template <typename T>
        requires std::derived_from<T, IMessure>
    void create(IMessure *property, const T &startValue, const T &endValue, float duration) {
        std::unique_ptr<IMessure> startValuePtr = std::make_unique<T>(startValue);
        std::unique_ptr<IMessure> endValuePtr = std::make_unique<T>(endValue);
        auto animRunner =
            std::make_unique<AnimationRunner<T>>(property, std::move(startValuePtr), std::move(endValuePtr), duration);
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

} // namespace gltk
