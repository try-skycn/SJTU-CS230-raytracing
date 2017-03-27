#pragma once 

#include "ray.hpp"
#include "shape.hpp"

struct Object {
	// member methods

	virtual IntersectionResult intersect(const Ray &ray) const = 0;
};
