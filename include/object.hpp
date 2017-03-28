#pragma once 

#include "ray.hpp"
#include "shape.hpp"

struct Object {
	// constructors & destructor

	virtual ~Object() {
	}

	// member methods

	virtual IntersectionResult intersect(const Ray &ray) const = 0;
};
