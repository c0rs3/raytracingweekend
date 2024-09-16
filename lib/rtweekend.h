#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <thread>

// C++ Std Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;
using std::fabs;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double uint32d_limit = static_cast<double>(UINT32_MAX);
const double pi = 3.1415926535897932385;
static uint32_t seed = 12346;

// Utility Functions

inline uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

inline double random_double_xorshift() {
    thread_local uint32_t seed = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    return xorshift32(seed) / uint32d_limit;
}

inline double random_double_xorshift(double min, double max) {
    return (xorshift32(seed) / uint32d_limit) * (max - min) + min;
}

inline int random_int(int min, int max) {
    return int(random_double_xorshift(min, max + 1));
}

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif