#pragma once

#include "rectangle_shape.hpp"
#include "normal_object.hpp"

struct RectangleObject : NormalObject {
	// members

	RectangleShape rectangle;

	// constructors

	RectangleObject(RectangleShape _rectangle, Material _material): NormalObject(_material), rectangle(_rectangle) {
	}

	// member methods

	Vec getNormal(const Vec &point) const final {
		return rectangle.getNormal();
	}

	IntersectionResult intersect(const Ray &ray) const final {
		return rectangle.intersect(ray);
	}
};
