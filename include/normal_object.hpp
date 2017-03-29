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

	// static member methods

	static RefractResult refract(const Vec &I, const Vec &N, float index) {
		float dot = I.dot(N);
		Vec L = (I - dot * N).unit(), NN = N;
		float inSin = I.dot(L), outSin;
		if (dot <= 0) {
			// positive
			outSin = inSin / index;
			NN = -N;
		} else {
			// negative
			outSin = inSin * index;
		}
		if (outSin < 1) {
			float outCos = std::sqrtf(1.0f - outSin * outSin);
			Vec out = (outSin * L + outCos * NN).unit();
			return {.survive = true, .dir = out};
		} else {
			return {.survive = false};
		}
	}

	static Vec reflect(const Vec &I, const Vec &N) { // input direction & normal vector
		return (I - 2.0 * I.dot(N) * N).unit();
	}
};
