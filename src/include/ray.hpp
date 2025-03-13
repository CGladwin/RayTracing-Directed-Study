#pragma once

#include "vec3.cpp"

/**
 * @brief A ray in 3D space.
 */
class ray {
  public:
    /**
     * @brief Default constructor.
     */
    constexpr ray() noexcept = default;

    /**
     * @brief Constructs a ray with the given origin and direction.
     * @param origin The origin of the ray.
     * @param direction The direction of the ray.
     */
    constexpr ray(const point3& origin, const vec3& direction) noexcept
        : orig(origin), dir(direction) {}

    /**
     * @brief Returns the origin of the ray.
     * @return The origin.
     */
    constexpr const point3& origin() const noexcept { return orig; }

    /**
     * @brief Returns the direction of the ray.
     * @return The direction.
     */
    constexpr const vec3& direction() const noexcept { return dir; }

    /**
     * @brief Computes the point along the ray at parameter t.
     * @param t The parameter.
     * @return The point at t.
     */
    constexpr point3 at(double t) const noexcept {
        return orig + t * dir;
    }

  private:
    point3 orig; // The origin of the ray.
    vec3 dir;    // The direction of the ray.
};