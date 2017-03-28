#pragma once

#include "planar_shape.hpp"
#include "ray.hpp"
#include "vec.hpp"

struct FinitePlanarShape : PlanarShape {
	// member methods

	virtual bool inside(const Vec &point) const = 0;

	IntersectionResult intersect(const Ray &ray) const final {
		IntersectionResult result = planarIntersect(ray);
		if (inside(result.hitPoint)) {
			return result;
		} else {
			return {.hit = false};
		}
	}
};
