#pragma once

#include <iostream>
#include <cmath>
#include <vector>

/**
 * @brief A 3D vector class.
 */
class vec3 {
  public:
    double e[3]; /// The components of the vector.

    /// Default constructor. Initializes all components to 0.
    constexpr vec3() noexcept;

    /// Constructs a vector with the given components.
    constexpr vec3(double e0, double e1, double e2) noexcept;

    /// Constructs a vector from a std::vector<float>.
    vec3(std::vector<float> v3) noexcept;

    constexpr double x() const noexcept;
    constexpr double y() const noexcept;
    constexpr double z() const noexcept;

    constexpr vec3 operator-() const noexcept;
    constexpr double operator[](int i) const noexcept;
    constexpr double& operator[](int i) noexcept;

    vec3& operator+=(const vec3& v) noexcept;
    vec3& operator*=(double t) noexcept;
    vec3& operator/=(double t) noexcept;

    constexpr double length() const noexcept;
    constexpr double length_squared() const noexcept;
    constexpr bool near_zero() const noexcept;

    static vec3 random() noexcept;
    static vec3 random(double min, double max) noexcept;
};

// Alias for vec3 to represent a point in 3D space.
using point3 = vec3;

// Utility functions
std::ostream& operator<<(std::ostream& out, const vec3& v) noexcept;
constexpr vec3 operator+(const vec3& u, const vec3& v) noexcept;
constexpr vec3 operator-(const vec3& u, const vec3& v) noexcept;
constexpr vec3 operator*(const vec3& u, const vec3& v) noexcept;
constexpr vec3 operator*(double t, const vec3& v) noexcept;
constexpr vec3 operator*(const vec3& v, double t) noexcept;
constexpr vec3 operator/(const vec3& v, double t) noexcept;
constexpr double dot(const vec3& u, const vec3& v) noexcept;
constexpr vec3 cross(const vec3& u, const vec3& v) noexcept;
inline vec3 unit_vector(const vec3& v) noexcept;
// inline vec3 random_in_unit_disk() noexcept;
// inline vec3 random_unit_vector() noexcept;
// inline vec3 random_on_hemisphere(const vec3& normal) noexcept;
// inline vec3 reflect(const vec3& v, const vec3& n) noexcept;
// inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) noexcept;
