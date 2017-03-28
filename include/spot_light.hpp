#pragma once

#include "light.hpp"

struct SpotLight : Light {
	// members

	Vec spot;

	// constructors

	SpotLight(const Vec &_spot, const Color &_color): Light(_color), spot(_spot) {
	}

	// member methods

	IntersectionResult intersect(const Ray &ray) const {
		if (ray.move((spot - ray.origin).dot(ray.dir)).dist(spot) < 0.01) {
			return IntersectionResult{.hit = true, .hitPoint = spot};
		} else {
			return IntersectionResult{.hit = false};
		}
	}
};
