#ifndef SIMD_H
#define SIMD_H
#pragma once
#include <immintrin.h>
class alignas(64) svec3{
public:
    float e[4];

    svec3() : e{0,0,0,0} {}
    svec3(float e0, float e1, float e2) : e{e0, e1, e2} {}
    svec3(const svec3& vec) {
        e[0] = vec.e[0];
        e[1] = vec.e[1];
        e[2] = vec.e[2];
    }

    svec3(svec3&& vec) noexcept {
        e[0] = vec.e[0];
        e[1] = vec.e[1];
        e[2] = vec.e[2];
    }

    svec3& operator=(const svec3& vec) {
        e[0] = vec.e[0];
        e[1] = vec.e[1];
        e[2] = vec.e[2];
        return *this;
    }

    svec3& operator=(svec3&& vec) noexcept {
        e[0] = vec.e[0];
        e[1] = vec.e[1];
        e[2] = vec.e[2];
        return *this;
    }

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    svec3 operator-() const { return svec3(-e[0], -e[1], -e[2]); }
    float operator[](int i) const { return e[i]; }
    float& operator[](int i) { return e[i]; }
    svec3& operator+=(const svec3& v) {
        __m128 ef = _mm_set_ps(0.0f, e[0], e[1], e[2]);
        __m128 ff = _mm_set_ps(0.0f, v[0], v[1], v[2]);

        __m128 res = _mm_add_ps(ef, ff);
        float resultArray[4];
        _mm_store_ps(e, res);
        
        return *this;
    }

    svec3& operator*=(float t) {
        __m128 ef = _mm_set_ps(0.0f, e[0], e[1], e[2]);
        __m128 ff = _mm_set_ps(0.0f, t, t, t);

        __m128 res = _mm_mul_ps(ef, ff);
        float resultArray[4];
        _mm_store_ps(e, res);
        return *this;
    }

    svec3& operator/=(float t) {
        return *this *= 1/t;
    }

    float length() const {
        return sqrt(length_squared());
    }

    float length_squared() const {
        __m128 ef = _mm_set_ps(0.0f, e[0], e[1], e[2]);
        __m128 res = _mm_mul_ps(ef, ef);
        float resultArray[4];
        _mm_store_ps(resultArray, res);
        return resultArray[0] + resultArray[1] + resultArray[2];
    }

    static svec3 random() {
        return svec3(random_double_xorshift(), random_double_xorshift(), random_double_xorshift());
    }

    static svec3 random(double min, double max) {
        return svec3(random_double_xorshift(min,max), random_double_xorshift(min,max), random_double_xorshift(min,max));
    }

    bool near_zero() const {
        // return true if the vector is close to zero in all dimensions.
        auto s = 1e-10;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }
};

inline std::ostream& operator<<(std::ostream& out, const svec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline svec3 operator+(const svec3& u, const svec3& v) {
    __m128 ef = _mm_set_ps(0.0f, u[0], u[1], u[2]);
    __m128 ff = _mm_set_ps(0.0f, v[0], v[1], v[2]);
    __m128 res = _mm_add_ps(ef, ff);
    float resultArray[4];
    _mm_store_ps(resultArray, res);
    return svec3(resultArray[0], resultArray[1], resultArray[2]);
}

inline svec3 operator-(const svec3& u, const svec3& v) {
    __m128 ef = _mm_set_ps(0.0f, u[0], u[1], u[2]);
    __m128 ff = _mm_set_ps(0.0f, v[0], v[1], v[2]);
    __m128 res = _mm_sub_ps(ef, ff);
    float resultArray[4];
    _mm_store_ps(resultArray, res);
    return svec3(resultArray[0], resultArray[1], resultArray[2]);
}

inline svec3 operator*(const svec3& u, const svec3& v) {
    __m128 ef = _mm_set_ps(0.0f, u[0], u[1], u[2]);
    __m128 ff = _mm_set_ps(0.0f, v[0], v[1], v[2]);
    __m128 res = _mm_mul_ps(ef, ff);
    float resultArray[4];
    _mm_store_ps(resultArray, res);
    return svec3(resultArray[0], resultArray[1], resultArray[2]);
}

inline svec3 operator*(float t, const svec3& v) {
    __m128 ef = _mm_set_ps(0.0f, v[0], v[1], v[2]);
    __m128 ff = _mm_set_ps(0.0f, t, t, t);
    __m128 res = _mm_mul_ps(ef, ff);
    float resultArray[4];
    _mm_store_ps(resultArray, res);
    return svec3(resultArray[0], resultArray[1], resultArray[2]);
}

inline float dot(const svec3& u, const svec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline svec3 cross(const svec3& u, const svec3& v) {
    return svec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline svec3 operator*(const svec3& v, float t) {
    return t * v;
}

inline svec3 operator/(const svec3& v, float t) {
    return (1/t) * v;
}

inline svec3 unit_vector(const svec3& v) {
    return v / v.length();
}

inline svec3 random_in_unit_sphere() {
    double theta = acos(1 - 2 * random_double_xorshift());   
    double phi = 2 * pi * random_double_xorshift();
    double r = cbrt(random_double_xorshift());   
    
    double x = r * sin(theta) * cos(phi);
    double y = r * sin(theta) * sin(phi);
    double z = r * cos(theta);

    return svec3(x, y, z);
}

inline svec3 random_in_unit_disk() {
    auto theta = 2.0 * pi * random_double_xorshift();
    auto r = std::sqrt(random_double_xorshift());

    auto x = r * std::cos(theta);
    auto y = r * std::sin(theta);

    return svec3(x, y, 0);
}

inline svec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline svec3 random_on_hemisphere(const svec3& normal) {
    svec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline svec3 reflect(const svec3& v, const svec3& n) {
    return v - 2*dot(v,n)*n;
}

inline svec3 refract(const svec3& uv, const svec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    svec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    svec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

//using vec3 = svec3;
//using point3 = vec3;

#endif