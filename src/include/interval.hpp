#pragma once

#include "common.hpp"

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
    
    constexpr double clamp(double x) const noexcept {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const interval empty, universe;
};

constexpr interval interval::empty    = interval{+infinity, 
                                                -infinity};
constexpr interval interval::universe = interval{-infinity, 
                                                +infinity};
