#pragma once

#include "shape.hpp"
#include "normal_object.hpp"

struct GeometryObject : NormalObject {
	// members

	Shape *shape;

	// constructors

	GeometryObject(Shape *_shape, Material _material): NormalObject(_material), shape(_shape) {
	}

	~GeometryObject() {
		delete(shape);
	}

	// member methods

	Vec getNormal(const Vec &point) const final {
		return shape->getNormal(point);
	}

	IntersectionResult intersect(const Ray &ray) const final {
		return shape->intersect(ray);
	}
};
