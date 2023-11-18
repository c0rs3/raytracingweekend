#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
        ray() {}
        ray(const point3d& origin, const vec3& direction) 
            : orig(origin), dir(direction) {}

        point3d origin() const  { return orig; }
        vec3 direction() const { return dir; }

        point3d at(double t) const {
            return orig + t*dir;
        }

    public:
        point3d orig;
        vec3 dir;
};

#endif