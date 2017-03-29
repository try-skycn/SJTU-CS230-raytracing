#pragma once

#include <vector>
#include "light.hpp"
#include "color.hpp"
#include "vec.hpp"
#include "rectangle_shape.hpp"

struct AreaLight : Light {
	// members

	RectangleShape rectangle;
	std::vector<Vec> samples;

	// constructors

	AreaLight(RectangleShape _rectangle, const Color &_color, int areaLightSamples): Light(_color), rectangle(_rectangle) {
		for (int i = 0; i < areaLightSamples; ++i) {
			samples.push_back(rectangle.sample());
		}
	}

	// member methods

	IntersectionResult intersect(const Ray &ray) const {
		return rectangle.intersect(ray);
	}
};
