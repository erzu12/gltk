#pragma once

#include "animation.h"
#include "vec_math.h"
#include <memory>

class IMessure {
  public:
    virtual float resolve(float parrentSize) = 0;
    virtual void setValue(float value) = 0;
    virtual float getValue() const = 0;
    virtual bool isAbsolute() const = 0;

    virtual void
    animateTo(const IMessure &targetValue, float durationSeconds, std::function<float(float)> easing = nullptr) = 0;
    virtual void animate(const IMessure &deltaValue) = 0;
    virtual void stopAnimation() = 0;
    virtual void update(float deltaTime) = 0;

    virtual ~IMessure() = default;
};

class AbsoluteMessure : public IMessure {
  private:
    gltk::Animatable<float> value;

  public:
    explicit AbsoluteMessure(int value) : value(value) {}
    explicit AbsoluteMessure(const AbsoluteMessure &other) : value(other.value) {}
    float resolve(float parrentSize) override;
    bool isAbsolute() const override { return true; }
    void setValue(float value) override { this->value.set(round(value)); }
    float getValue() const override { return static_cast<float>(round(value.get())); }

    void animateTo(
        const IMessure &targetValue, float durationSeconds, std::function<float(float)> easing = nullptr
    ) override {
        if (!targetValue.isAbsolute()) {
            throw std::invalid_argument("Cannot animate AbsoluteMessure to RelativeMessure");
        }
        value.animateTo(targetValue.getValue(), durationSeconds, easing);
    }
    void animate(const IMessure &deltaValue) override {
        if (!deltaValue.isAbsolute()) {
            throw std::invalid_argument("Cannot animate AbsoluteMessure by RelativeMessure");
        }
        value.animate(deltaValue.getValue());
    }
    void stopAnimation() override { value.stopAnimation(); }
    void update(float deltaTime) override { value.update(deltaTime); }

    AbsoluteMessure operator-() const { return AbsoluteMessure(-value.get()); }
};

class RelativeMessure : public IMessure {
  private:
    gltk::Animatable<float> value;

  public:
    float resolve(float parrentSize) override;
    explicit RelativeMessure(float value) : value(value) {}
    bool isAbsolute() const override { return false; }
    void setValue(float value) override { this->value.set(value); }
    float getValue() const override { return value.get(); }

    void animateTo(
        const IMessure &targetValue, float durationSeconds, std::function<float(float)> easing = nullptr
    ) override {
        if (targetValue.isAbsolute()) {
            throw std::invalid_argument("Cannot animate RelativeMessure to AbsoluteMessure");
        }
        value.animateTo(targetValue.getValue(), durationSeconds, easing);
    }
    void animate(const IMessure &deltaValue) override {
        if (deltaValue.isAbsolute()) {
            throw std::invalid_argument("Cannot animate RelativeMessure by AbsoluteMessure");
        }
        value.animate(deltaValue.getValue());
    }
    void stopAnimation() override { value.stopAnimation(); }
    void update(float deltaTime) override { value.update(deltaTime); }

    RelativeMessure operator-() const { return RelativeMessure(-value.get()); }
};

struct MessureVec2 {
    std::unique_ptr<IMessure> x = nullptr;
    std::unique_ptr<IMessure> y = nullptr;

    MessureVec2() : x(std::make_unique<AbsoluteMessure>(0)), y(std::make_unique<AbsoluteMessure>(0)) {}
    MessureVec2(const AbsoluteMessure &x_, const AbsoluteMessure &y_)
        : x(std::make_unique<AbsoluteMessure>(x_)), y(std::make_unique<AbsoluteMessure>(y_)) {}

    MessureVec2(const RelativeMessure &x_, const RelativeMessure &y_)
        : x(std::make_unique<RelativeMessure>(x_)), y(std::make_unique<RelativeMessure>(y_)) {}

    MessureVec2(AbsoluteMessure x_, RelativeMessure y_)
        : x(std::make_unique<AbsoluteMessure>(x_)), y(std::make_unique<RelativeMessure>(y_)) {}

    MessureVec2(RelativeMessure x_, AbsoluteMessure y_)
        : x(std::make_unique<RelativeMessure>(x_)), y(std::make_unique<AbsoluteMessure>(y_)) {}

    Vec2 resolve(Vec2 parentSize);

    void update(float deltaTime) {
        x->update(deltaTime);
        y->update(deltaTime);
    }

    std::unique_ptr<IMessure> &operator[](int index);
};

AbsoluteMessure operator""_px(unsigned long long int value);
RelativeMessure operator""_percent(long double value);
RelativeMessure operator""_pct(long double value);
RelativeMessure operator""_percent(unsigned long long int value);
RelativeMessure operator""_pct(unsigned long long int value);
