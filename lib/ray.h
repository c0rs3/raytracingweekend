#ifndef RAY_H
#define RAY_H

#include "vec3.h"
// #include "simd.h"
class ray {
  public:
    ray() {}

    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    ray(const point3& origin, const vec3& direction, double time) : orig(origin), dir(direction), time(time) {}

    const point3& origin() const  { return orig; }
    const vec3& direction() const { return dir; }

    point3 at(double t) const {
        return orig + t*dir;
    }

    double get_time() const{
        return time;
    }

  private:
    point3 orig;
    vec3 dir;
    double time;
};

#endif