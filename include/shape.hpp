#pragma once

#include "ray.hpp"
#include "vec.hpp"

struct IntersectionResult {
	bool hit;
	Vec hitPoint;
};

struct Shape {
	// constructors & destructor

	virtual ~Shape() {
	}

	// member methods

	virtual IntersectionResult intersect(const Ray &ray) const = 0;
	virtual Vec getNormal(const Vec &hitPoint) const = 0;
};
