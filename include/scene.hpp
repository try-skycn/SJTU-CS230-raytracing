#pragma once

#include <vector>
#include "vec.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "light.hpp"

struct FirstHitResult {
	bool hit;
	Object *object;
	Vec point;
};

struct Scene {
	// members

	std::vector<Object *> objects;
	std::vector<Light *> lights;

	// member methods

	void addObject(Object *object) {
		objects.push_back(object);
		if (Light *light = dynamic_cast<Light *>(object)) {
			lights.push_back(light);
		}
	}

	FirstHitResult firstHit(const Ray &ray) const {
		FirstHitResult result = {.hit = false};
		float dist = 0.0;
		for (Object *object : objects) {
			IntersectionResult intersectionResult = object->intersect(ray);
			float newDist = ray.origin.dist(intersectionResult.point);
			if (intersectionResult.hit && (!result.hit || newDist < dist)) {
				result.hit = true;
				result.object = object;
				result.point = intersectionResult.point;
				dist = newDist;
			}
		}
		return result;
	}
};
