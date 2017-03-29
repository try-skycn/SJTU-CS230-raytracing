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

	const Scene &scene;
	const TraceConfig &config;
	const Ray &ray;
	const int depth;

	Vec H, N, R; // hit point & normal vector & reflection vector

	// constructor

	Tracer(const Scene &_scene, const TraceConfig &_config, const Ray &_ray, int _depth = 5):
			scene(_scene), config(_config), ray(_ray), depth(_depth) {
	}

	Tracer(const Tracer &parent, const Ray &_ray, int _depth):
			scene(parent.scene), config(parent.config), ray(_ray), depth(_depth) {
	}

	Tracer(const Tracer &parent, const Ray &_ray):
			scene(parent.scene), config(parent.config), ray(_ray), depth(parent.depth - 1) {
	}

	// trace

	Color trace(FirstHitResult defaultHit = {.hit = false}) {
		if (depth <= 0) return Color();
		FirstHitResult firstHitResult = scene.firstHit(ray, defaultHit);
		if (firstHitResult.hit) {
			H = firstHitResult.hitPoint;
			return lightDecay(resolveHit(firstHitResult.object), firstHitResult.dist);
		}
		return Color();
	}

	// light decay

	Color lightDecay(const Color &color, float dist) const {
		return color;
	}

	// resolve hit

	Color resolveHit(Object *object) {
		if (Light *light = dynamic_cast<Light *>(object)) {
			return light->color;
		} else if (NormalObject *normalObject = dynamic_cast<NormalObject *>(object)) {
			if (depth > 1) {
				N = normalObject->getNormal(H);
				R = NormalObject::reflect(ray.dir, N);
				return shading(normalObject->material) +
				       diffuseReflection(normalObject->material) +
				       reflection(normalObject->material) +
				       refraction(normalObject->material);
			}
		} else assert(false);
		return Color();
	}

	// shading

	Color shading(const Material &material) const {
		if (material.kShading > 0) {
			Color result;
			for (Light *light : scene.lights) {
				if (AreaLight *areaLight = dynamic_cast<AreaLight *>(light)) {
					result += resolveShading(material, areaLight);
				} else if (SpotLight *spotLight = dynamic_cast<SpotLight *>(light)) {
					result += resolveShading(material, spotLight);
				} else assert(false);
			}
			return result * material.kShading;
		}
		return Color();
	}

	// resolve shading

	Color resolveShading(const Material &material, AreaLight *areaLight) const {
		Color result;
		for (const Vec &sample : areaLight->samples) {
			result += resolveSpotShading(material, static_cast<Light *>(areaLight), sample);
		}
		return result / static_cast<float>(areaLight->samples.size());
	}

	Color resolveShading(const Material &material, SpotLight *spotLight) const {
		return resolveSpotShading(material, static_cast<Light *>(spotLight), spotLight->getCenter());
	}

	// diffuse reflection

	Color diffuseReflection(const Material &material) const {
		if (material.kDiffuseReflection > 0) {
			Vec L = N.rotate(), U = N.cross(L);
			Color result;
			for (Vec V : scene.shadings) {
				result += Tracer(*this, Ray(H, V.x * N + V.y * L + V.z * U), std::min(depth - 1, 2)).trace();
			}
			return result / scene.shadings.size() * material.kDiffuseReflection;
		}
		return Color();
	}

	// reflection

	Color reflection(const Material &material) const {
		if (material.kReflection > 0) return Tracer(*this, Ray(H, R)).trace() * material.kReflection;
		return Color();
	}

	// refraction

	Color refraction(const Material &material) const {
		if (material.kRefraction > 0) {
			RefractResult refractResult = NormalObject::refract(ray.dir, N, material.index);
			if (refractResult.survive) return Tracer(*this, Ray(H, refractResult.dir)).trace() * material.kRefraction;
		}
		return Color();
	}

	// resolve shading color

	Color resolveSpotShading(const Material &material, Light *light, const Vec &spot) const {
		Ray tmpRay(H, spot - H);
		if (tmpRay.dir.dot(N) > 0) {
			return (
					       material.kDiffuseShading * material.color * N.dot(tmpRay.dir) +
					       material.kSpecularShading * std::powf(std::fmaxf(R.dot(tmpRay.dir), 0.0f), 20.0)
			       ) * Tracer(*this, tmpRay, std::min(depth - 1, 1)).trace({.hit = true, .object = light, .hitPoint = spot, .dist = tmpRay.origin.dist(spot)});
		}
		return Color();
	}
};
