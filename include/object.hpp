#pragma once 

#include "material.hpp"
#include "ray.hpp"

struct IntersectionResult {
	bool hit;
	float dist;
};

// struct Object {
// static Vec get_refracted(const Vec &in_dir, const Vec &normal, float in_index, float out_index) { assert(false); return Vec(); }
// };

struct Object {
	// members

	Material material;

	// constructors

	Object(const Material &_material): material(_material) {
	}

	virtual IntersectionResult intersect(const Ray &ray) const = 0;

	virtual Vec getNormal(const Vec &point) const = 0;

	Vec reflect(const Vec &inDir, const Vec &point) {
		Vec normal = getNormal(point);
		return (inDir - 2.0 * inDir.dot(normal) * normal).unit();
	}
};
