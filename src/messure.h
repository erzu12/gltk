#pragma once

#include "vec_math.h"
#include <memory>

class IMessure {
  public:
    virtual float resolve(float parrentSize) = 0;
    virtual void setValue(float value) = 0;
    virtual float getValue() const = 0;
    virtual bool isAbsolute() = 0;
    virtual ~IMessure() = default;
};

class AbsoluteMessure : public IMessure {
  private:
    int value;

  public:
    explicit AbsoluteMessure(int value) : value(value) {}
    explicit AbsoluteMessure(const AbsoluteMessure &other) : value(other.value) {}
    float resolve(float parrentSize) override;
    bool isAbsolute() override { return true; }
    void setValue(float value) override { this->value = round(value); }
    float getValue() const override { return static_cast<float>(value); }

    AbsoluteMessure operator-() const { return AbsoluteMessure(-value); }
};

class RelativeMessure : public IMessure {
  private:
    float value;

  public:
    float resolve(float parrentSize) override;
    explicit RelativeMessure(float value) : value(value) {}
    bool isAbsolute() override { return false; }
    void setValue(float value) override { this->value = value; }
    float getValue() const override { return value; }

    RelativeMessure operator-() const { return RelativeMessure(-value); }
};

struct MessureVec2 {
    std::unique_ptr<IMessure> x;
    std::unique_ptr<IMessure> y;

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

    std::unique_ptr<IMessure> &operator[](int index);
};

AbsoluteMessure operator""_px(unsigned long long int value);
RelativeMessure operator""_percent(long double value);
RelativeMessure operator""_pct(long double value);
RelativeMessure operator""_percent(unsigned long long int value);
RelativeMessure operator""_pct(unsigned long long int value);
