#pragma once
#include <limits>

constexpr double infinity = std::numeric_limits<double>::infinity();

class interval {
  public:
    double min, max;

    constexpr interval() noexcept 
        : min(+infinity), 
          max(-infinity) {}

    constexpr interval(double min, double max) noexcept 
        : min(min), max(max) {}

    constexpr double size() const noexcept {
        return max - min;
    }

    constexpr bool contains(double x) const noexcept {
        return min <= x && x <= max;
    }

    constexpr bool surrounds(double x) const noexcept {
        return min < x && x < max;
    }

    static const interval empty, universe;
};

constexpr interval interval::empty    = interval{+infinity, 
                                                -infinity};
constexpr interval interval::universe = interval{-infinity, 
                                                +infinity};
