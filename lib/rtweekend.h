#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

static std::uniform_real_distribution<> distribution(0.0, 1.0);
static uint32_t seed = 12346;

// C++ Std Usings
static std::random_device rd;
static std::mt19937 generator(rd());

using std::make_shared;
using std::shared_ptr;
using std::sqrt;
using std::fabs;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

inline double random_double_xorshift(uint32_t& seed) {
    return xorshift32(seed) / static_cast<double>(UINT32_MAX);
}

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    return distribution(generator);
}

inline double random_double(double min, double max) {
    static std::uniform_real_distribution<> distribution(min, max);
    return distribution(generator);
}


// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif