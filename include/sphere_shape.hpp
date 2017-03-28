#pragma once

#include "shape.hpp"

struct SphereShape : Shape {
	// members

	Vec center;
	float radius;

	// constructors

	SphereShape(const Vec &_center, float _radius): center(_center), radius(_radius) {
	}

	// member methods

	Vec getNormal(const Vec &hitPoint) const {
		return (hitPoint - center).unit();
	}

	IntersectionResult intersect(const Ray &ray) const {
		float F = (center - ray.origin).dot(ray.dir);
		Vec P = ray.move(F);
		float h = center.dist(P);
		float sqrD = radius * radius - h * h;
		if (sqrD < -EPS) {
			return {.hit = false};
		} else if (sqrD < EPS) {
			return {.hit = true, .hitPoint = P};
		} else {
			float D = std::sqrtf(sqrD);
			if (F - D >= 0) {
				return {.hit = true, .hitPoint = ray.move(F - D)};
			} else if (F + D >= 0) {
				return {.hit = true, .hitPoint = ray.move(F + D)};
			} else {
				return {.hit = false};
			}
		}
	}
};