#pragma once

#include "resources.hpp"
#include "planar_shape.hpp"

struct PlaneShape : PlanarShape {
	// members

	Vec base, normal;

	// constructors

	PlaneShape(const Vec &_base, const Vec &_normal): base(_base), normal(_normal.unit()) {
	}

	// member methods

	Vec getBasePoint() const final {
		return base;
	}

	Vec getNormal() const final {
		return normal;
	}

	IntersectionResult intersect(const Ray &ray) const {
		return planarIntersect(ray);
	}
};
