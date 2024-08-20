#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

static std::uniform_real_distribution<> distribution(0.0, 1.0);

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