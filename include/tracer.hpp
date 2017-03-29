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

	Color trace(const Ray &ray, int depth=5) const {
		if (depth == 0) {
			return Color();
		}
		FirstHitResult firstHitResult = scene->firstHit(ray);
		if (firstHitResult.hit) {
			return resolveHit(ray, firstHitResult.object, firstHitResult.hitPoint, depth);
		} else {
			return Color();
		}
	}

	// resolve hit

	Color resolveHit(const Ray &ray, Object *object, const Vec &point, int depth) const {
		if (Light *light = dynamic_cast<Light *>(object)) {
			return resolveHit(ray, light, point, depth);
		} else if (NormalObject *normalObject = dynamic_cast<NormalObject *>(object)) {
			return resolveHit(ray, normalObject, point, depth);
		} else {
			assert(false);
			return Color();
		}
	}

	Color resolveHit(const Ray &ray, Light *light, const Vec &hitPoint, int depth) const {
		return light->getLightStrength(ray, hitPoint, config->lightDecayCoeff);
	}

	Color resolveHit(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint, int depth) const {
		return shading(ray, normalObject, hitPoint) +
				reflection(ray, normalObject, hitPoint, depth) +
				refraction(ray, normalObject, hitPoint, depth);
	}

	// shading

	Color shading(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint) const {
		if (normalObject->material.kShading > 0) {
			Color result;
			for (Light *light : scene->lights) {
				result += resolveShading(ray, normalObject, hitPoint, light);
			}
			return result * normalObject->material.kShading;
		} else {
			return Color();
		}
	}

	// resolve shading

	Color resolveShading(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint, Light *light) const {
		if (AreaLight *areaLight = dynamic_cast<AreaLight *>(light)) {
			return resolveShading(ray, normalObject, hitPoint, areaLight);
		} else if (SpotLight *spotLight = dynamic_cast<SpotLight *>(light)) {
			return resolveShading(ray, normalObject, hitPoint, spotLight);
		} else {
			assert(false);
			return Color();
		}
	}

	Color resolveSpotShading(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint, Light *light, const Vec &spot) const {
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

	Color resolveShading(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint, AreaLight *areaLight) const {
		Color result;
		for (const Vec &sample : areaLight->samples) {
			result += resolveSpotShading(ray, normalObject, hitPoint, static_cast<Light *>(areaLight), sample);
		}
		return result / static_cast<float>(areaLight->samples.size());
	}

	Color resolveShading(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint, SpotLight *spotLight) const {
		return resolveSpotShading(ray, normalObject, hitPoint, static_cast<Light *>(spotLight), spotLight->getCenter());
	}

	// reflection

	Color reflection(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint, int depth) const {
		if (normalObject->material.kReflection > 0) {
			return trace(Ray(hitPoint, normalObject->reflect(ray.dir, hitPoint)), depth - 1) * normalObject->material.kReflection;
		} else {
			return Color();
		}
	}

	// refraction

	Color refraction(const Ray &ray, NormalObject *normalObject, const Vec &hitPoint, int depth) const {
		if (normalObject->material.kRefraction > 0) {
			RefractResult refractResult = normalObject->refract(ray.dir, hitPoint);
			if (refractResult.survive) {
				return trace(Ray(hitPoint, refractResult.dir), depth - 1) * normalObject->material.kRefraction;
			} else {
				return Color();
			}
		} else {
			return Color();
		}
	}
};
