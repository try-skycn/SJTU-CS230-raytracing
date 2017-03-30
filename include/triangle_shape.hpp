#pragma once

#include "resources.hpp"
#include "finite_planar_shape.hpp"

struct TriangleShape : FinitePlanarShape {
	// members

	Vec start, height, width;

	// constructors

	TriangleShape(const Vec &a, const Vec &b, const Vec &c): start(b), height(a - b), width(c - b) {
	}

	// member methods

	Vec getBasePoint() const final {
		return start;
	}

	Vec getNormal() const final {
		return width.cross(height).unit();
	}

	bool inside(const Vec &point) const final {
		Vec D = point - start, ND = D - width, N = getNormal();
		return width.cross(D).dot(N) >= 0 &&
		       D.cross(height).dot(N) >= 0 &&
		       (height - width).cross(ND).dot(N) >= 0 &&
		       ND.cross(-width).dot(N) >= 0;
	}
};
