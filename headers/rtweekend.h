#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <thread>

using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

static constexpr auto RINFINITY = std::numeric_limits<float>::infinity();
static constexpr auto uint32d_limit = static_cast<float>(UINT32_MAX);
static constexpr auto pi = 3.1415926535897932385f;

// Utility Functions

constexpr inline uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max,
                              uint32_t out_min, uint32_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

inline float random_float_xorshift() {
    thread_local uint32_t seed =
        static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    return xorshift32(seed) / uint32d_limit;
}

inline float random_float_xorshift(float min, float max) {
    thread_local uint32_t seed =
        static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    return (xorshift32(seed) / uint32d_limit) * (max - min) + min;
}

inline int random_int(int min, int max) {
    return int(random_float_xorshift(min, max + 1));
}

constexpr inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0f;
}