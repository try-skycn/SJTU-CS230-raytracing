#pragma once

#include <cassert>
#include <cmath>
#include "vec.hpp"
#include "color.hpp"
#include "scene.hpp"
#include "ray.hpp"
#include "light.hpp"
#include "normal_object.hpp"
#include "area_light.hpp"
#include "spot_light.hpp"
#include "traceconfig.hpp"

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

	Color resolveHit(const Ray &ray, Light *light, const Vec &hitPoint) {
		return light->getLightStrength(ray, hitPoint, config->lightDecayCoeff);
	}

	Color resolveHit(const Ray &ray, NormalObject *normalObject, const Vec &point) {
		return shading(ray, normalObject, point);
	}

	// shading

	Color shading(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint) {
		Color result;
		for (Light *light : scene->lights) {
			result += resolveShading(ray, normalObject, hitPoint, light);
		}
		return result;
	}

	// resolve shading

	Color resolveShading(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint, Light *light) {
		if (AreaLight *areaLight = dynamic_cast<AreaLight *>(light)) {
			return resolveShading(ray, normalObject, hitPoint, areaLight);
		} else if (SpotLight *spotLight = dynamic_cast<SpotLight *>(light)) {
			return resolveShading(ray, normalObject, hitPoint, spotLight);
		} else {
			assert(false);
			return Color();
		}
	}

	Color resolveSpotShading(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint, Light *light, const Vec &spot) {
		Ray tmpRay(hitPoint, spot - hitPoint);
		if (tmpRay.dir.dot(normalObject->getNormal(hitPoint)) < 0) {
			return Color();
		}
		FirstHitResult firstHitResult = scene->firstHit(tmpRay);
		if (firstHitResult.hit && firstHitResult.object == light) {
			Color color = light->getLightStrength(tmpRay, firstHitResult.hitPoint, config->lightDecayCoeff), result;
			if (normalObject->material.kDiffuseShading > 0) {
				result += color * normalObject->material.kDiffuseShading *
						normalObject->getNormal(hitPoint).dot(tmpRay.dir) * normalObject->material.color;
			}
			if (normalObject->material.kSpecularShading > 0) {
				float dot = normalObject->reflect(ray.dir, hitPoint).dot(tmpRay.dir);
				result += color * normalObject->material.kSpecularShading *
						powf(std::fmaxf(dot, 0.0f), 20);
			}
			return result;
		} else {
			return Color(0, 0, 0);
		}
	}

	Color resolveShading(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint, AreaLight *areaLight) {
		Color result;
		for (int i = 0; i < config->areaLightSamples; ++i) {
			result += resolveSpotShading(ray, normalObject, hitPoint, static_cast<Light *>(areaLight),
			                             areaLight->rectangle.sample());
		}
		return result / static_cast<float>(config->areaLightSamples);
	}

	Color resolveShading(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint, SpotLight *spotLight) {
		return resolveSpotShading(ray, normalObject, hitPoint, static_cast<Light *>(spotLight), spotLight->getCenter());
	}
};
