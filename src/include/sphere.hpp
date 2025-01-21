#include "vec3.hpp"
struct Sphere {
    vec3 center;
    float radius;

    Sphere(const vec3 &c, const float &r) : center(c), radius(r) {}

    bool ray_intersect(const vec3 &orig, const vec3 &dir, float &t0) const {
        vec3 L = center - orig;
        float tca = L*dir;
        float d2 = L*L - tca*tca;
        if (d2 > radius*radius) return false;
        float thc = sqrtf(radius*radius - d2);
        t0       = tca - thc;
        float t1 = tca + thc;
        if (t0 < 0) t0 = t1;
        if (t0 < 0) return false;
        return true;
    }
};