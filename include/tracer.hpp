#include <cassert>
#include "vec.hpp"
#include "color.hpp"
#include "scene.hpp"
#include "ray.hpp"
#include "light.hpp"
#include "normal_object.hpp"
#include "area_light.hpp"
#include "spot_light.hpp"

struct TraceConfig {
	float light_decay_coeff = 3.0;
	int divHeight = 15, divWidth = 15;
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
			return resolveHit(ray, firstHitResult.object, firstHitResult.hitPoint);
		} else {
			return Color();
		}
	}

	// resolve hit

	Color resolveHit(const Ray &ray, Object *object, const Vec &point) {
		if (Light *light = dynamic_cast<Light *>(object)) {
			return resolveHit(ray, light, point);
		} else if (NormalObject *normalObject = dynamic_cast<NormalObject *>(object)) {
			return resolveHit(ray, normalObject, point);
		} else {
			assert(false);
			return Color();
		}
	}

	Color resolveHit(const Ray &ray, Light *light, const Vec &point) {
		return light->color / ray.origin.dist(point) * config->light_decay_coeff;
//		return light->color;
	}

	Color resolveHit(const Ray &ray, NormalObject *normalObject, const Vec &point) {
		return shading(normalObject, point) * normalObject->material.color;
	}

	// shading

	Color shading(NormalObject *normalObject, const Vec &point) {
		Color result;
		for (Light *light : scene->lights) {
			result += resolveShading(normalObject, light, point);
		}
		return result;
	}

	// resolve shading

	Color resolveShading(NormalObject *normalObject, Light *light, const Vec &point) {
		if (AreaLight *areaLight = dynamic_cast<AreaLight *>(light)) {
			return resolveShading(normalObject, areaLight, point);
		} else if (SpotLight *spotLight = dynamic_cast<SpotLight *>(light)) {
			return resolveShading(normalObject, spotLight, point);
		} else {
			assert(false);
			return Color();
		}
	}

	Color resolveShading(NormalObject *normalObject, AreaLight *areaLight, const Vec &point) {
		RectangleIterator iterator = areaLight->rectangle.getIterator(config->divHeight, config->divWidth);
		Color result;
		for (int i = 0; i < config->divHeight; ++i) {
			for (int j = 0; j < config->divWidth; ++j) {
				Ray tmpRay(point, iterator.get(i, j) - point);
				FirstHitResult firstHitResult = scene->firstHit(tmpRay);
				if (firstHitResult.hit && firstHitResult.object == areaLight) {
					result += resolveHit(tmpRay, static_cast<Light *>(areaLight), firstHitResult.hitPoint);
				}
			}
		}
		return result / (static_cast<float>(config->divHeight) * static_cast<float>(config->divWidth));
	}

	Color resolveShading(NormalObject *normalObject, SpotLight *spotLight, const Vec &point) {
		Ray tmpRay(point, spotLight->spot - point);
		FirstHitResult firstHitResult = scene->firstHit(tmpRay);
		if (firstHitResult.hit && firstHitResult.object == spotLight) {
			return resolveHit(tmpRay, static_cast<Light *>(spotLight), firstHitResult.hitPoint);
		} else {
			return Color(0, 0, 0);
		}
	}
};
