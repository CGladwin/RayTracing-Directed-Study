#pragma once

#include <iosfwd>
#include <fstream>
#include <string>
#include <stdexcept>
#include <variant>
#include <cmath>
#include <memory>
#include <random> 

// Common Headers

#include "color.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"

// C++ Std Usings

using std::make_shared;
using std::shared_ptr;
using std::cout;
using std::string;

// Constants



// defined in "interval.hpp"
// #include <limits>
// const double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;
constexpr double degrees_to_radians_factor = pi / 180.0; // Precompute for performance

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * degrees_to_radians_factor;
}

// Thread-local random number generator
inline std::mt19937& get_random_generator() {
    thread_local std::mt19937 generator(std::random_device{}());
    return generator;
}

// Returns a random real in [0, 1)
inline double random_double() {
    thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(get_random_generator());
}

// Returns a random real in [min, max)
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template <typename Val, typename... Ts>
auto match(Val val, Ts... ts) { 
    // https://en.cppreference.com/w/cpp/utility/variant/visit
    return std::visit(overloaded{ts...}, val);  
}