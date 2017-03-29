#pragma once

#include "light.hpp"

struct SpotLight : Light {
	// members

	Vec spot;

	// constructors

	SpotLight(const Vec &_spot, const Color &_color): Light(_color), spot(_spot) {
	}

	// member methods

	Vec getCenter() const {
		return spot;
	}

	IntersectionResult intersect(const Ray &ray) const {
		return {.hit = false};
	}
};
