#pragma once

#include <vector>
#include "vec.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "light.hpp"
#include "normal_object.hpp"

struct FirstHitResult {
	bool hit;
	Object *object;
	Vec hitPoint;
	float dist;

	void update(Object *_object, const Vec &_hitPoint, float _dist) {
		if (!hit || _dist < dist) {
			hit = true;
			object = _object;
			hitPoint = _hitPoint;
			dist = _dist;
		}
	}
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

	FirstHitResult firstHit(const Ray &ray, FirstHitResult defaultResult = {.hit = false}) const {
		bool skipLight = defaultResult.hit;
		for (Object *object : objects) {
			if (skipLight && dynamic_cast<Light *>(object)) continue;
			IntersectionResult intersectionResult = object->intersect(ray);
			if (intersectionResult.hit) {
				float dist = ray.origin.dist(intersectionResult.hitPoint);
				defaultResult.update(object, intersectionResult.hitPoint, dist);
			}
		}
		return defaultResult;
	}
};
