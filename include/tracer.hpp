#include "vec.hpp"
#include "color.hpp"
#include "scene.hpp"
#include "ray.hpp"

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

	Color resolveHit(Object *object, const Vec &point);
};
