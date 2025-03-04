#pragma once

#include "common.hpp"

/**
 * @brief A 3D vector class.
 */
class vec3 {
  public:
    double e[3]; /// The components of the vector.

    /**
     * @brief Default constructor. Initializes all components to 0.
     */
    constexpr vec3() noexcept : e{0, 0, 0} {}

    vec3(std::vector<float> v3) noexcept : e{v3[0],v3[1],v3[2]} {}

    /**
     * @brief Constructs a vector with the given components.
     * @param e0 The x-component.
     * @param e1 The y-component.
     * @param e2 The z-component.
     */
    constexpr vec3(double e0, double e1, double e2) noexcept : e{e0, e1, e2} {}

    /**
     * @brief Returns the x-component of the vector.
     * @return The x-component.
     */
    constexpr double x() const noexcept { return e[0]; }

    /**
     * @brief Returns the y-component of the vector.
     * @return The y-component.
     */
    constexpr double y() const noexcept { return e[1]; }

    /**
     * @brief Returns the z-component of the vector.
     * @return The z-component.
     */
    constexpr double z() const noexcept { return e[2]; }

    /**
     * @brief Negates the vector.
     * @return The negated vector.
     */
    constexpr vec3 operator-() const noexcept { return vec3(-e[0], -e[1], -e[2]); }

    /**
     * @brief Accesses the i-th component of the vector (const version).
     * @param i The index of the component.
     * @return The i-th component.
     */
    constexpr double operator[](int i) const noexcept { return e[i]; }

    /**
     * @brief Accesses the i-th component of the vector (non-const version).
     * @param i The index of the component.
     * @return A reference to the i-th component.
     */
    constexpr double& operator[](int i) noexcept { return e[i]; }

    /**
     * @brief Adds another vector to this vector.
     * @param v The vector to add.
     * @return A reference to this vector.
     */
    vec3& operator+=(const vec3& v) noexcept {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    /**
     * @brief Multiplies this vector by a scalar.
     * @param t The scalar.
     * @return A reference to this vector.
     */
    vec3& operator*=(double t) noexcept {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    /**
     * @brief Divides this vector by a scalar.
     * @param t The scalar.
     * @return A reference to this vector.
     */
    vec3& operator/=(double t) noexcept {
        return *this *= 1 / t;
    }

    /**
     * @brief Computes the length (magnitude) of the vector.
     * @return The length of the vector.
     */
    constexpr double length() const noexcept {
        return std::sqrt(length_squared());
    }

    /**
     * @brief Computes the squared length of the vector.
     * @return The squared length of the vector.
     */
    constexpr double length_squared() const noexcept {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    /**
     * @brief Return true if the vector is close to zero in all dimensions.
     * 
     * @return true 
     * @return false 
     */
    constexpr bool near_zero() const noexcept {
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    /**
     * @brief Generates a random vector with components in [0, 1).
     * @return A random vector.
     */
    static vec3 random() noexcept {
        return vec3(random_double(), random_double(), random_double());
    }

    /**
     * @brief Generates a random vector with components in [min, max).
     * @param min The lower bound.
     * @param max The upper bound.
     * @return A random vector.
     */
    static vec3 random(double min, double max) noexcept {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

/// Alias for vec3 to represent a point in 3D space.
using point3 = vec3;

// Vector Utility Functions


inline std::ostream& operator<<(std::ostream& out, const vec3& v) noexcept {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}


inline constexpr vec3 operator+(const vec3& u, const vec3& v) noexcept {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}


inline constexpr vec3 operator-(const vec3& u, const vec3& v) noexcept {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

/**
 * @brief Multiplies two vectors component-wise.
 * @param u The first vector.
 * @param v The second vector.
 * @return The component-wise product.
 */
inline constexpr vec3 operator*(const vec3& u, const vec3& v) noexcept {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

/**
 * @brief Multiplies a scalar by a vector.
 * @param t The scalar.
 * @param v The vector.
 * @return The scaled vector.
 */
inline constexpr vec3 operator*(double t, const vec3& v) noexcept {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

/**
 * @brief Multiplies a vector by a scalar.
 * @param v The vector.
 * @param t The scalar.
 * @return The scaled vector.
 */
inline constexpr vec3 operator*(const vec3& v, double t) noexcept {
    return t * v;
}

/**
 * @brief Divides a vector by a scalar.
 * @param v The vector.
 * @param t The scalar.
 * @return The scaled vector.
 */
inline constexpr vec3 operator/(const vec3& v, double t) noexcept {
    return (1 / t) * v;
}

/**
 * @brief 
 * @param u The first vector.
 * @param v The second vector.
 * @return The dot product.
 */
inline constexpr double dot(const vec3& u, const vec3& v) noexcept {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

/**
 * @brief 
 * @param u The first vector.
 * @param v The second vector.
 * @return The cross product.
 */
inline constexpr vec3 cross(const vec3& u, const vec3& v) noexcept {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

/**
 * @brief Normalizes a given vector, so that its length is 1
 * @param v The vector.
 * @return The unit vector.
 */
inline vec3 unit_vector(const vec3& v) noexcept {
    return v / v.length();
}

inline vec3 random_in_unit_disk() noexcept {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

/**
 * @brief uses rejection method; indefinitely loops until it 
 * generates a vec3 that falls within sphere, then normalizes it
 * @return A random unit vector.
 */
inline vec3 random_unit_vector() noexcept {
    while (true) {
        auto p = vec3::random(-1, 1);
        auto lensq = p.length_squared(); // doesn't use length at this point to avoid sqrt early
        if (1e-160 < lensq && lensq <= 1)
            return p / std::sqrt(lensq);
    }
}

/**
 * @brief Generates a random vector on the hemisphere defined by the given normal.
 * @param normal The normal vector defining the hemisphere.
 * @return A random vector on the hemisphere.
 */
inline vec3 random_on_hemisphere(const vec3& normal) noexcept {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) noexcept {
    return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) noexcept {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}