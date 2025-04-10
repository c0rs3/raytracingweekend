#ifndef RAY_H
#define RAY_H

#ifdef USE_SIMD
#include "simd.h"
#else
#include "vec3.h"
#endif
class ray {
public:
	ray() {}

	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

	ray(const point3& origin, const vec3& direction, float time) : orig(origin), dir(direction), time(time) {}

	const point3& origin() const { return orig; }
	const vec3& direction() const { return dir; }

	point3 at(float t) const {
		return orig + t * dir;
	}

	float get_time() const {
		return time;
	}

private:
	point3 orig;
	vec3 dir;
	float time;
};

#endif