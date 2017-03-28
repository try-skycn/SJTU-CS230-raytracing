#pragma once

#include <vector>
#include "vec.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "light.hpp"

struct FirstHitResult {
	bool hit;
	Object *object;
	Vec hitPoint;
};

struct Scene {
	// members

	std::vector<Object *> objects;
	std::vector<Light *> lights;

	// constructors & destructor

	~Scene() {
		lights.clear();
		for (Object *object : objects) {
			delete(object);
		}
		objects.clear();
	}

	// member methods

	void addObject(Object *object) {
		objects.push_back(object);
		if (Light *light = dynamic_cast<Light *>(object)) {
			lights.push_back(light);
		}
	}

	FirstHitResult firstHit(const Ray &ray) const {
		FirstHitResult firstHitResult = {.hit = false};
		float dist = 0.0;
		for (Object *object : objects) {
			IntersectionResult intersectionResult = object->intersect(ray);
			float newDist = ray.origin.dist(intersectionResult.hitPoint);
			if (intersectionResult.hit && (!firstHitResult.hit || newDist < dist)) {
				firstHitResult.hit = true;
				firstHitResult.object = object;
				firstHitResult.hitPoint = intersectionResult.hitPoint;
				dist = newDist;
			}
		}
		return firstHitResult;
	}
};
