#pragma once

#include "vec_math.h"
#include <memory>

class IMessure {
  public:
    virtual float resolve(float parrentSize) = 0;
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
};

class RelativeMessure : public IMessure {
  private:
    float value;

  public:
    explicit RelativeMessure(float value) : value(value) {}
    float resolve(float parrentSize) override;
    bool isAbsolute() override { return false; }
};

struct MessureVec2 {
    std::unique_ptr<IMessure> x;
    std::unique_ptr<IMessure> y;

    MessureVec2(const AbsoluteMessure &x_, const AbsoluteMessure &y_)
        : x(std::make_unique<AbsoluteMessure>(x_)), y(std::make_unique<AbsoluteMessure>(y_)) {}

    MessureVec2(const RelativeMessure &x_, const RelativeMessure &y_)
        : x(std::make_unique<RelativeMessure>(x_)), y(std::make_unique<RelativeMessure>(y_)) {}

    MessureVec2(AbsoluteMessure x_, RelativeMessure y_)
        : x(std::make_unique<AbsoluteMessure>(x_)), y(std::make_unique<RelativeMessure>(y_)) {}

    MessureVec2(RelativeMessure x_, AbsoluteMessure y_)
        : x(std::make_unique<RelativeMessure>(x_)), y(std::make_unique<AbsoluteMessure>(y_)) {}

    Vec2 resolve(Vec2 parentSize);
};

AbsoluteMessure operator""_px(unsigned long long int value);
RelativeMessure operator""_percent(long double value);
RelativeMessure operator""_pct(long double value);
RelativeMessure operator""_percent(unsigned long long int value);
RelativeMessure operator""_pct(unsigned long long int value);
