#pragma once

#include "vec.hpp"
#include "ray.hpp"
#include "screen.hpp"
#include "scene.hpp"
#include "tracer.hpp"

struct Camera {
	// members

	Vec origin, center, dx, dy;
	Screen screen;

	// constructors

	Camera(const Ray &sight, const Vec &up, float dist, int height, int width, float delta):
			origin(sight.origin), center(sight.move(dist)),
			dx(-up * delta), dy(sight.dir.cross(up) * delta),
			screen(height, width){
	}

	void see(Scene *scene, TraceConfig *config) {
		Tracer *tracer = new Tracer(scene, config);
		for (int i = 0; i < screen.height; ++i) {
			for (int j = 0; j < screen.width; ++j) {
				Vec screenPoint = center +
						(-static_cast<float>(screen.height) / 2.0 + 0.5 + static_cast<float>(i)) * dx +
						(-static_cast<float>(screen.width) / 2.0 + 0.5 + static_cast<float>(j)) * dy;
				screen.store(i, j, tracer->trace(Ray(origin, screenPoint - origin)));
			}
		}
	}
};
