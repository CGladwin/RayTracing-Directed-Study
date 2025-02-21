#pragma once

#include "common.hpp"

/**
 * @brief Represents an interval [min, max].
 */
class interval {
  public:
    double min; ///< The lower bound of the interval.
    double max; ///< The upper bound of the interval.

    /**
     * @brief Default constructor. Creates an empty interval.
     */
    constexpr interval() noexcept : min(+infinity), max(-infinity) {}

    /**
     * @brief Constructs an interval with the given bounds.
     * @param min The lower bound.
     * @param max The upper bound.
     */
    constexpr interval(double min, double max) noexcept : min(min), max(max) {}

    /**
     * @brief Computes the size of the interval.
     * @return The size (max - min).
     */
    constexpr double size() const noexcept {
        return max - min;
    }

    /**
     * @brief Checks if the interval contains a value.
     * @param x The value to check.
     * @return True if x is in [min, max], false otherwise.
     */
    constexpr bool contains(double x) const noexcept {
        return min <= x && x <= max;
    }

    /**
     * @brief Checks if the interval surrounds a value (exclusive bounds).
     * @param x The value to check.
     * @return True if x is in (min, max), false otherwise.
     */
    constexpr bool surrounds(double x) const noexcept {
        return min < x && x < max;
    }

    /**
     * @brief Clamps a value to the interval.
     * @param x The value to clamp.
     * @return The clamped value.
     */
    constexpr double clamp(double x) const noexcept {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    constexpr interval expand(double delta) const noexcept {
        auto padding = delta/2;
        return interval(min - padding, max + padding);
    }

    /// Represents an empty interval.
    static const interval empty;

    /// Represents the interval (-∞, +∞).
    static const interval universe;
};

constexpr interval interval::empty = interval{+infinity, -infinity};
constexpr interval interval::universe = interval{-infinity, +infinity};