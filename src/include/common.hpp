#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <variant>
#include <cmath>
#include <memory>
#include <random>
#include <limits>

// C++ Std Usings
using std::make_shared;
using std::shared_ptr;
using std::cout;
using std::string;

// Constants
constexpr double infinity = std::numeric_limits<double>::infinity(); ///< Represents positive infinity.
constexpr double pi = 3.1415926535897932385; ///< The value of π.
constexpr double degrees_to_radians_factor = pi / 180.0; ///< Precomputed factor for converting degrees to radians.

/**
 * @brief Converts degrees to radians.
 * @param degrees The angle in degrees.
 * @return The angle in radians.
 */
inline constexpr double degrees_to_radians(double degrees) noexcept {
    return degrees * degrees_to_radians_factor;
}

/**
 * @brief 
 * @return A reference to a thread-local Mersenne Twister random number generator.
 */
inline std::mt19937& get_random_generator() noexcept {
    thread_local std::mt19937 generator(std::random_device{}());
    return generator;
}

/**
 * @brief 
 * @return A random double in [0, 1).
 */
inline double random_double() noexcept {
    thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(get_random_generator());
}

/**
 * @brief Returns a random real number in the range [min, max).
 * @param min The lower bound (inclusive).
 * @param max The upper bound (exclusive).
 * @return A random double in [min, max).
 */
inline double random_double(double min, double max) noexcept {
    return min + (max - min) * random_double();
}

/**
 * @brief A helper struct for overloading arbitrary function objects Ts (e.g., for std::visit).
 */
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

// Deduction guide for overloaded (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

/**
 * @brief A utility function to simplify std::visit usage with overloaded.
 * @param val The variant to visit.
 * @param ts The overloaded function objects.
 * @return The result of the matching function.
 */
template <typename Val, typename... Ts>
auto match(Val val, Ts... ts) {
    return std::visit(overloaded{ts...}, val);
}