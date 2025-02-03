#pragma once

#include <iosfwd>
#include <fstream>
#include <string>
#include <stdexcept>
#include <variant>
#include <cmath>
#include <memory>

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

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}


template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template <typename Val, typename... Ts>
auto match(Val val, Ts... ts) { 
    // https://en.cppreference.com/w/cpp/utility/variant/visit
    return std::visit(overloaded{ts...}, val);  
}