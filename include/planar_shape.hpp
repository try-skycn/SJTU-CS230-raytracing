#pragma once

#include "shape.hpp"
#include "ray.hpp"

struct PlanarShape : Shape {
	// member methods

	virtual Vec getBasePoint() const = 0;

	virtual Vec getNormal() const = 0;

	Vec planarIntersect(const Ray &ray) const final {
		Vec D = getBasePoint() - ray.origin, N = getNormal();
		return ray.move(D.dot(N) / ray.dir.dot(N));
	}
};
