#pragma once

#include "object.hpp"
#include "material.hpp"

// struct Object {
// static Vec get_refracted(const Vec &in_dir, const Vec &normal, float in_index, float out_index) { assert(false); return Vec(); }
// };

struct NormalObject : Object {
	// members

	Material material;

	// constructors

	NormalObject(const Material &_material): material(_material) {
	}

	// member methods

	virtual Vec getNormal(const Vec &point) const = 0;

	Vec reflect(const Vec &inDir, const Vec &point) const {
		Vec normal = getNormal(point);
		return (inDir - 2.0 * inDir.dot(normal) * normal).unit();
	}
};
