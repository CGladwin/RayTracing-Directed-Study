#pragma once

#include "ray.hpp"
#include "interval.hpp"
#include "aabb.hpp"

class material;

class hit_record {
  public:
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;
    double t;
    double u;
    double v;
    bool front_face;

    // Sets the hit record normal vector.
    // NOTE: the parameter `outward_normal` is assumed to have unit length.
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

/**
 * @brief abstract base class for hittable objects in 3d space
 */
class hittable {
  public:
    virtual ~hittable() = default;

    // pure virtual function; must be overridden in derived classes
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

    virtual aabb bounding_box() const = 0;
};