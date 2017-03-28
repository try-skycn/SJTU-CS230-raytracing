#pragma once

#include "object.hpp"
#include "color.hpp"

struct Light : Object {
	// members

	Color color;

	// member methods

	Light(const Color &_color): color(_color) {
	}

	// member methods

	Color getColor(const Ray &ray, const Vec &hitPoint, float lightDecayCoeff) const {
		return color / ray.origin.sqrdist(hitPoint) * lightDecayCoeff;
	}

	virtual Color getLightStrength(const Ray &ray, const Vec &hitPoint, float lightDecayCoeff) const {
		return getColor(ray, hitPoint, lightDecayCoeff);
	}
};
