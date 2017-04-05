#pragma once

#include <cmath>
#include <algorithm>

constexpr float EPS = 1e-4;

#ifdef __linux__
namespace std {
	#define powf pow
	#define sinf sin
	#define sqrtf sqrt
	#define cosf cos
	#define expf exp
}
#endif

struct Vec {
	// members

	float x, y, z;

	// constructors

	Vec(): x(0.), y(0.), z(0.) {
	}

	Vec(float _x): x(_x), y(_x), z(_x) {
	}

	Vec(float _x, float _y, float _z): x(_x), y(_y), z(_z) {
	}

	// assignment operations

	Vec &operator+=(const Vec &r) {
		x += r.x; y += r.y; z += r.z; return *this;
	}

	Vec &operator-=(const Vec &r) {
		x -= r.x; y -= r.y; z -= r.z; return *this;
	}

	Vec &operator*=(const Vec &r) {
		x *= r.x; y *= r.y; z *= r.z; return *this;
	}

	Vec &operator/=(const Vec &r) {
		x /= r.x; y /= r.y; z /= r.z; return *this;
	}

	Vec &operator*=(float r) {
		x *= r; y *= r; z *= r; return *this;
	}

	Vec &operator/=(float r) {
		x /= r; y /= r; z /= r; return *this;
	}

	// operations

	Vec operator+(const Vec &r) const {
		return Vec(x + r.x, y + r.y, z + r.z);
	}

	Vec operator-(const Vec &r) const {
		return Vec(x - r.x, y - r.y, z - r.z);
	}

	Vec operator*(const Vec &r) const {
		return Vec(x * r.x, y * r.y, z * r.z);
	}

	Vec operator/(const Vec &r) const {
		return Vec(x / r.x, y / r.y, z / r.z);
	}

	Vec operator*(float r) const {
		return Vec(x * r, y * r, z * r);
	}

	Vec operator/(float r) const {
		return Vec(x / r, y / r, z / r);
	}

	Vec operator-() const {
		return Vec(-x, -y, -z);
	}

	friend Vec operator*(float l, const Vec &r) {
		return Vec(l * r.x, l * r.y, l * r.z);
	}

	// functional operations

	float dot(const Vec &r) const {
		return x * r.x + y * r.y + z * r.z;
	}

	Vec cross(const Vec &r) const {
		return Vec(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
	}

	float sqrlen() const {
		return dot(*this);
	}

	float len() const {
		return std::sqrtf(sqrlen());
	}

	Vec unit() const {
		return *this / len();
	}

	float sqrdist(const Vec &r) const {
		return (r - *this).sqrlen();
	}

	float dist(const Vec &r) const {
		return (r - *this).len();
	}

	Vec toward(const Vec &r) const {
		return (r - *this).unit();
	}

	Vec rotate() const {
		float fx = fabsf(x), fy = fabsf(y), fz = fabsf(z);
		if (fx <= fy && fx <= fz) {
			return cross(Vec(x, z, -y)).unit();
		} else if (fy <= fx && fy <= fz) {
			return cross(Vec(z, y, -x)).unit();
		} else {
			return cross(Vec(y, -x, z)).unit();
		}
	}

	friend Vec fminf(const Vec &l, const Vec &r) {
		return Vec(std::fminf(l.x, r.x), std::fminf(l.y, r.y), std::fminf(l.z, r.z));
	}

	friend Vec fminf(const Vec &l, float r) {
		return Vec(std::fminf(l.x, r), std::fminf(l.y, r), std::fminf(l.z, r));
	}

	friend Vec expf(const Vec &r) {
		return Vec(std::expf(r.x), std::expf(r.y), std::expf(r.z));
	}
};
