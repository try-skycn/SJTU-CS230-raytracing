#pragma once

#include "ray.hpp"
#include "vec.hpp"

struct IntersectionResult {
	bool hit;
	Vec hitPoint;
};

struct Shape {
	// member methods

	virtual IntersectionResult intersect(const Ray &ray) const = 0;
};
