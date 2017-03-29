#pragma once

#include "object.hpp"
#include "material.hpp"

struct RefractResult {
	bool survive;
	Vec dir;
};

struct NormalObject : Object {
	// members

	Material material;

	// constructors

	NormalObject(const Material &_material): material(_material) {
	}

	// member methods

	virtual Vec getNormal(const Vec &hitPoint) const = 0;

	Vec reflect(const Vec &inDir, const Vec &hitPoint) const {
		Vec normal = getNormal(hitPoint);
		return (inDir - 2.0 * inDir.dot(normal) * normal).unit();
	}

	RefractResult refract(const Vec &inDir, const Vec &hitPoint) const {
		Vec N = getNormal(hitPoint);
		float dot = inDir.dot(N);
		Vec L = (inDir - dot * N).unit();
		float inSin = inDir.dot(L), outSin;
		if (dot <= 0) {
			// positive
			outSin = inSin / material.index;
			N = -N;
		} else {
			// negative
			outSin = inSin * material.index;
		}
		if (outSin < 1) {
			float outCos = std::sqrtf(1.0f - outSin * outSin);
			return {.survive = true, .dir = (outSin * L + outCos * N).unit()};
		} else {
			return {.survive = false};
		}
	}
};
