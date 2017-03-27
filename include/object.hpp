#pragma once 

#include "material.hpp"
#include "ray.hpp"

struct IntersectionResult {
	bool hit;
	float dist;
};

struct Object {
	// member methods

	virtual IntersectionResult intersect(const Ray &ray) const = 0;
};
