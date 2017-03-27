#pragma once

#include "vec.hpp"

struct Ray {
	// members

	Vec origin, dir;

	// constructors

	Ray(const Vec &_origin, const Vec &_dir): origin(_origin), dir(_dir.unit()) {
		origin += EPS * dir;
	}

	// member methods

	Vec move(float dist) const {
		return origin + dir * dist;
	}

	bool isForward(const Vec &point) const {
		return (point - origin).dot(dir) >= 0;
	}
};
