#pragma once

#include "finite_planar_shape.hpp"

struct RectangleShape : FinitePlanarShape {
	// members

	Vec start, height, width;

	// constructors

	RectangleShape(const Vec &a, const Vec &b, const vec &c): start(b), height(a - b), width(c - b) {
	}

	// member methods

	Vec getBasePoint() const final {
		return start;
	}

	Vec getNormal() const final {
		return width.cross(height).unit();
	}
};
