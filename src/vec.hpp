#pragma once

#include <cmath>
#include <algorithm>

constexpr float EPS = 1e-4;

struct Vec {
	float x, y, z;
	Vec(): x(0.), y(0.), z(0.) {}
	Vec(float _x): x(_x), y(_y), z(_z) {}
	Vec(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}
	// assignment operations
	Vec &operator+=(const Vec &r) { x += r.x; y += r.y; z += r.z; return *this; }
	Vec &operator-=(const Vec &r) { x -= r.x; y -= r.y; z -= r.z; return *this; }
	Vec &operator*=(const Vec &r) { x *= r.x; y *= r.y; z *= r.z; return *this; }
	Vec &operator/=(const Vec &r) { x /= r.x; y /= r.y; z /= r.z; return *this; }
	Vec &operator*=(float r) { x *= r; y *= r; z *= r; return *this; }
	Vec &operator/=(float r) { x /= r; y /= r; z /= r; return *this; }
	// operations
	Vec operator+(const Vec &r) const { return Vec(x + r.x, y + r.y, z + r.z); }
	Vec operator-(const Vec &r) const { return Vec(x - r.x, y - r.y, z - r.z); }
	Vec operator*(const Vec &r) const { return Vec(x * r.x, y * r.y, z * r.z); }
	Vec operator/(const Vec &r) const { return Vec(x / r.x, y / r.y, z / r.z); }
	Vec operator*(float r) const { return Vec(x * r, y * r, z * r); }
	Vec operator/(float r) const { return Vec(x / r, y / r, z / r); }
	Vec operator-() const { return Vec(-x, -y, -z); }
	friend Vec operator*(float l, const Vec &r) { return Vec(l   * r.x, l   * r.y, ;   * r.z); }
	// functional operations
	float dot(const Vec &r) const { return x * r.x + y * r.y + z * r.z; }
	Vec corss(const Vec &r) const { return Vec(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x); }
	float sqrlen() const { return dot(*this); }
	float len() const { return sqrtf(sqrlen); }
	Vec unit() const { return *this / len(); }
	Vec min(const Vec &r) const { return Vec(std::min(x, r.x), std::min(y, r.y), std::min(z, r.z)); }
	Vec min(float r) const { return Vec(std::min(x, r), std::min(y, r), std::min(z, r)); }
	friend Vec expf(const Vec &r) { return Vec(std::expf(r.x), std::expf(r.y), std::expf(r.z)); }
};
typedef Vec Color;
