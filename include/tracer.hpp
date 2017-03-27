#include <cassert>
#include "vec.hpp"
#include "color.hpp"
#include "scene.hpp"
#include "ray.hpp"
#include "light.hpp"

struct TraceConfig {

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
			return resolveHit(firstHitResult.object, firstHitResult.point);
		} else {
			return Color();
		}
	}

	// resolve hit

	Color resolveHit(Object *object, const Vec &point) {
		if (Light *light = dynamic_cast<Light *>(object)) {
			return resolveHit(light, point);
		} else {
			assert(false);
			return Color();
		}
	}

	Color resolveHit(Light *object, const Vec &point) {
		return object->color;
	}
};
