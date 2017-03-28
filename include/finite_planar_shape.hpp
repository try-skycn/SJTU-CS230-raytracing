#pragma once

#include "planar_shape.hpp"
#include "ray.hpp"
#include "vec.hpp"

struct FinitePlanarShape : PlanarShape {
	// member methods

	virtual bool inside(const Vec &point) const = 0;

	IntersectionResult intersect(const Ray &ray) const final {
		Vec hitPoint = planarIntersect(ray);
		return {.hit = inside(hitPoint) && ray.isForward(hitPoint), .hitPoint = hitPoint};
	}
};
