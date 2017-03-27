#include <cassert>
#include "vec.hpp"
#include "color.hpp"
#include "scene.hpp"
#include "ray.hpp"
#include "light.hpp"

struct TraceConfig {
	float light_decay_coeff = 8.0;
};

struct Tracer {
	// members

	Scene *scene;
	TraceConfig *config;

	// constructor

	Tracer(Scene *_scene, TraceConfig *_config): scene(_scene), config(_config) {
	}

	// trace

	Color trace(const Ray &ray) {
		FirstHitResult firstHitResult = scene->firstHit(ray);
		if (firstHitResult.hit) {
			return resolveHit(ray, firstHitResult.object, firstHitResult.point);
		} else {
			return Color();
		}
	}

	// resolve hit

	Color resolveHit(const Ray &ray, Object *object, const Vec &point) {
		if (Light *light = dynamic_cast<Light *>(object)) {
			return resolveHit(ray, light, point);
		} else {
			assert(false);
			return Color();
		}
	}

	Color resolveHit(const Ray &ray, Light *object, const Vec &point) {
		return object->color / ray.origin.sqrdist(point) * config->light_decay_coeff;
	}
};
