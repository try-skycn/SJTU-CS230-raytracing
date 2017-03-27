#pragma once

#include <vector>
#include "vec.hpp"
#include "object.hpp"
#include "ray.hpp"

struct FirstHitResult {
	bool hit;
	Object *object;
	Vec point;
};

struct Scene {
	// members

	std::vector<Object *> objects;

	// member methods

	void addObject(Object *object) {
		objects.push_back(object);
	}

	FirstHitResult firstHit(const Ray &ray) const {
		FirstHitResult result = {.hit = false};
		float dist = 0.0;
		for (Object *object : objects) {
			IntersectionResult intersectionResult = object->intersect(ray);
			if (intersectionResult.hit && (!result.hit || ray.origin.dist(intersectionResult.point) < dist)) {
				result.hit = true;
				result.object = object;
				result.point = ray.move(dist);
			}
		}
		return result;
	}
};
