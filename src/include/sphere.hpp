#pragma once

#include "hittable.hpp"
#include "vec3.cpp"

/**
 * @brief a sphere in 3d space
 * 
 * inherits from "hittable"
 * 
 */
struct sphere : public hittable {
  sphere() : center(point3()), radius(0.0), mat(nullptr) {
    auto rvec = vec3(radius, radius, radius);
    bbox = aabb(center - rvec, center + rvec);
  }

  sphere(const point3& center, double radius, shared_ptr<material> mat)
    : center(std::move(center)), radius(std::move(std::fmax(0,radius))), mat(std::move(mat)) 
    {
      auto rvec = vec3(radius, radius, radius);
      bbox = aabb(center - rvec, center + rvec);
    }

  /**
   * @brief uses ray sphere intersection formula to determine if sphere has been hit by ray-cast
   * 
   * @param r 
   * @param ray_tmin 
   * @param ray_tmax 
   * @param rec 
   * @return true 
   * @return false 
   */
  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
      vec3 oc = center - r.origin();
      auto a = r.direction().length_squared();
      auto h = dot(r.direction(), oc);
      auto c = oc.length_squared() - radius*radius;

      auto discriminant = h*h - a*c;
      if (discriminant < 0)
          return false;

      auto sqrtd = std::sqrt(discriminant);

      // Find the nearest root that lies in the acceptable range.
      // discards solutions that get too close to 0 
      auto root = (h - sqrtd) / a;
      if (!ray_t.surrounds(root)) {
          root = (h + sqrtd) / a;
          if (!ray_t.surrounds(root))
              return false;
      }

      rec.t = root;
      rec.p = r.at(rec.t);
      vec3 outward_normal = (rec.p - center) / radius;
      rec.set_face_normal(r, outward_normal);
      rec.mat = mat;

      return true;
  }

  point3 center;
  double radius;
  shared_ptr<material> mat;
  aabb bbox;
};