#pragma once 

#include "material.hpp"
#include "ray.hpp"

struct IntersectionResult {

};

struct Object {
	// members

	Material material;

	// constructors

	Object(const Material &_material): material(_material) {
	}

	virtual IntersectionResult intersect(const Ray &ray) const = 0;

	virtual Vec get_normal(const Vec &point) const = 0;
};
