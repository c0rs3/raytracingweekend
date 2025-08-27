#if 0
#ifndef SIMD_H
#define SIMD_H
#pragma once
#include <immintrin.h>
#include <iostream>
#include <cmath>

class alignas(16) svec3 {
public:
	__m128 e;

	svec3() : e{ _mm_setzero_ps() } {}
	svec3(float e0, float e1, float e2) : e{ _mm_set_ps(0.0f, e2, e1, e0) } {}
	svec3(__m128 e) : e(e) {}

	svec3(const svec3& vec) : e(vec.e) {}
	svec3(svec3&& vec) noexcept : e(vec.e) {}

	svec3& operator=(const svec3& vec) {
		e = std::move(vec.e);
		return *this;
	}

	svec3& operator=(svec3&& vec) noexcept {
		e = std::move(vec.e);
		return *this;
	}

	float x() const { return _mm_cvtss_f32(_mm_shuffle_ps(e, e, _MM_SHUFFLE(0, 0, 0, 0))); }
	float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(e, e, _MM_SHUFFLE(1, 1, 1, 1))); }
	float z() const { return _mm_cvtss_f32(_mm_shuffle_ps(e, e, _MM_SHUFFLE(2, 2, 2, 2))); }

	svec3 operator-() const { return svec3(_mm_sub_ps(_mm_setzero_ps(), e)); }

	inline float operator[](int i) const {
		if (i < 0 || i > 2) return 0.0f; // Handle out-of-bounds
		switch (i) {
		case 0: return x();
		case 1: return y();
		case 2: return z();
		default: return 0.0f;
		}
	}

	inline float& operator[](int i) {
		switch (i) {
		case 0: {
			float&& a = this->x();
			return a;
		}
		case 1: {
			float&& b = this->y();
			return b;
		}
		case 2: {
			float&& c = this->z();
			return c;
		}
		}
	}

	inline svec3& operator+=(const svec3& v) {
		e = _mm_add_ps(e, v.e);
		return *this;
	}

	inline svec3& operator*=(const float& t) {
		__m128 ff = _mm_set1_ps(t);
		e = _mm_mul_ps(e, ff);
		return *this;
	}

	inline svec3& operator/=(float t) {
		return *this *= 1 / t;
	}

	inline float length() const {
		return sqrt(length_squared());
	}
	/*
	inline float length_squared() const {
		__m128 res = _mm_mul_ps(e, e);
		svec3 temp(res);
		return temp.x() + temp.y() + temp.z();
	}
	*/
	inline float length_squared() const {
		__m128 res = _mm_mul_ps(e, e);  // e * e
		__m128 hadd = _mm_hadd_ps(res, res);  // Horizontal add (sum the first 2, then the second 2 components)
		hadd = _mm_hadd_ps(hadd, hadd);  // Final horizontal add to sum all components
		return _mm_cvtss_f32(hadd);  // Extract the final sum
	}

	inline static svec3 random() {
		return svec3(random_float_xorshift(), random_float_xorshift(), random_float_xorshift());
	}

	inline static svec3 random(float min, float max) {
		return svec3(random_float_xorshift(min, max), random_float_xorshift(min, max), random_float_xorshift(min, max));
	}

	bool near_zero() const {
		double s = 1e-10;
		return (fabs(this->x()) < s) && (fabs(this->y()) < s) && (fabs(this->z()) < s);
	}
};

inline std::ostream& operator<<(std::ostream& out, const svec3& v) {
	return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline svec3 operator+(const svec3& u, const svec3& v) {
	return svec3(_mm_add_ps(u.e, v.e));
}

inline svec3 operator-(const svec3& u, const svec3& v) {
	return svec3(_mm_sub_ps(u.e, v.e));
}

inline svec3 operator*(const svec3& u, const svec3& v) {
	return svec3(_mm_mul_ps(u.e, v.e));
}

inline svec3 operator*(float t, const svec3& v) {
	return svec3(_mm_mul_ps(_mm_set1_ps(t), v.e));
}

inline float dot(const svec3& u, const svec3& v) {
	return _mm_cvtss_f32(_mm_dp_ps(u.e, v.e, 0x71));  // 0x71 keeps only first 3 components and sums them
}

inline svec3 cross(const svec3& u, const svec3& v) {
	__m128 u_yzx = _mm_shuffle_ps(u.e, u.e, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 v_yzx = _mm_shuffle_ps(v.e, v.e, _MM_SHUFFLE(3, 0, 2, 1));

	__m128 mult1 = _mm_mul_ps(u_yzx, v.e);
	__m128 mult2 = _mm_mul_ps(v_yzx, u.e);

	__m128 result = _mm_sub_ps(mult1, mult2);
	result = _mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 0, 2, 1));

	return svec3(result);
}

inline svec3 operator*(const svec3& v, float t) {
	return t * v;
}

inline svec3 operator/(const svec3& v, float t) {
	return (1 / t) * v;
}

inline svec3 unit_vector(const svec3& v) {
	return v / v.length();
}

inline svec3 random_in_unit_sphere() {
	float theta = acos(1 - 2 * random_float_xorshift());
	float phi = 2 * pi * random_float_xorshift();
	float r = cbrt(random_float_xorshift());

	float x = r * sin(theta) * cos(phi);
	float y = r * sin(theta) * sin(phi);
	float z = r * cos(theta);

	return svec3(x, y, z);
}

inline svec3 random_in_unit_disk() {
	auto theta = 2.0 * pi * random_float_xorshift();
	auto r = std::sqrt(random_float_xorshift());

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
	return v - 2 * dot(v, n) * n;
}

inline svec3 refract(const svec3& uv, const svec3& n, float etai_over_etat) {
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	svec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	svec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

using vec3 = svec3;
using point3 = vec3;

void test_svec3() {
	std::cout << "=== Testing svec3 ===\n";

	// Indexing
	svec3 a(1.0f, 2.0f, 3.0f);
	std::cout << "a = (" << a.x() << ", " << a.y() << ", " << a.z() << ")\n";

	// Ngation
	svec3 neg_a = -a;
	std::cout << "-a = (" << neg_a.x() << ", " << neg_a.y() << ", " << neg_a.z() << ")\n";

	// Addition
	svec3 b(4.0f, 5.0f, 6.0f);
	svec3 c = a + b;
	std::cout << "a + b = (" << c.x() << ", " << c.y() << ", " << c.z() << ")\n";

	// Subtraction
	c = a - b;
	std::cout << "a - b = (" << c.x() << ", " << c.y() << ", " << c.z() << ")\n";

	// Multiplication
	c = a * b;
	std::cout << "a * b = (" << c.x() << ", " << c.y() << ", " << c.z() << ")\n";

	// Scalar multiplication
	c = a * 2.0f;
	std::cout << "a * 2 = (" << c.x() << ", " << c.y() << ", " << c.z() << ")\n";

	// Scalar division
	c = a / 2.0f;
	std::cout << "a / 2 = (" << c.x() << ", " << c.y() << ", " << c.z() << ")\n";

	// Dot product
	float dot_product = dot(a, b);
	std::cout << "dot(a, b) = " << dot_product << "\n";

	// Cross product
	c = cross(a, b);
	std::cout << "cross(a, b) = (" << c.x() << ", " << c.y() << ", " << c.z() << ")\n";

	// Length
	std::cout << "length(a) = " << a.length() << "\n";

	// Normalization
	c = unit_vector(a);
	std::cout << "unit_vector(a) = (" << c.x() << ", " << c.y() << ", " << c.z() << ")\n";

	// near_zero
	svec3 small(1e-11f, 1e-11f, 1e-11f);
	std::cout << "small is near zero: " << (small.near_zero() ? "true" : "false") << "\n";

	// Reflection
	svec3 normal(0.0f, 1.0f, 0.0f);
	c = reflect(a, normal);
	std::cout << "reflect(a, normal) = (" << c.x() << ", " << c.y() << ", " << c.z() << ")\n";

	// Refraction
	c = refract(a, normal, 0.5f);
	std::cout << "refract(a, normal, 0.5) = (" << c.x() << ", " << c.y() << ", " << c.z() << ")\n";

	// Random vectors
	svec3 rand_vec = svec3::random();
	std::cout << "random vector = (" << rand_vec.x() << ", " << rand_vec.y() << ", " << rand_vec.z() << ")\n";

	svec3 rand_unit = random_unit_vector();
	std::cout << "random_unit_vector = (" << rand_unit.x() << ", " << rand_unit.y() << ", " << rand_unit.z() << ")\n";
	std::cout << a.x() << a.y() << a.z() << std::endl;
	std::cout << "=== End of Tests ===\n";
}

#endif
#endif