#pragma once

#include "light.hpp"

struct SpotLight : Light {
	// members

	static float radius, boundry;
	Vec spot;

	// constructors

	SpotLight(const Vec &_spot, const Color &_color): Light(_color), spot(_spot) {
	}

	// member methods

	Vec getCenter() const {
		return spot;
	}

	virtual Color getLightStrength(const Ray &ray, const Vec &hitPoint, float lightDecayCoeff) const {
		float h = ray.move((spot - ray.origin).dot(ray.dir)).dist(spot);
		return getColor(ray, hitPoint, lightDecayCoeff) * (1.0f - h * boundry / radius);
	}

	IntersectionResult intersect(const Ray &ray) const {
		Vec P = ray.move((spot - ray.origin).dot(ray.dir));
		if (ray.isForward(P) && P.dist(spot) < radius) {
			return IntersectionResult{.hit = true, .hitPoint = spot};
		} else {
			return IntersectionResult{.hit = false};
		}
	}
};

float SpotLight::radius = 0.01;
float SpotLight::boundry = 0.95;
