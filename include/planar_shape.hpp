#pragma once

#include "shape.hpp"
#include "ray.hpp"

struct PlanarShape : Shape {
	// member methods

	virtual Vec getBasePoint() const = 0;

	virtual Vec getNormal() const = 0;

	Vec getNormal(const Vec &hitPoint) const {
		return getNormal();
	}

	IntersectionResult planarIntersect(const Ray &ray) const {
		Vec D = getBasePoint() - ray.origin, N = getNormal();
		float dot = ray.dir.dot(N);
		if (-EPS < dot && dot < EPS) {
			return {.hit = false};
		} else {
			Vec hitPoint = ray.move(D.dot(N) / dot);
			if (ray.isForward(hitPoint)) {
				return {.hit = true, .hitPoint = hitPoint};
			} else {
				return {.hit = false};
			}
		}
	}
};
