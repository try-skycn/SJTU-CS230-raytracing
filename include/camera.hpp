#pragma once

#include "vec.hpp"
#include "ray.hpp"
#include "screen.hpp"
#include "scene.hpp"
#include "tracer.hpp"
#include "rectangle_shape.hpp"

struct Camera {
	// members

	Vec origin;
	RectangleShape rectangle;

	// constructors

	Camera(const Ray &sight, const Vec &up, float dist, float height, float width):
			origin(sight.origin),
			rectangle(sight.move(dist) + (height * 0.5f) * up - (width * 0.5f) * sight.dir.cross(up),
			          sight.move(dist) - (height * 0.5f) * up - (width * 0.5f) * sight.dir.cross(up),
			          sight.move(dist) - (height * 0.5f) * up + (width * 0.5f) * sight.dir.cross(up)) {
	}

	void see(Screen *screen, Scene *scene, TraceConfig *config) {
		RectangleIterator iterator = rectangle.getIterator(screen->height, screen->width);
		Tracer *tracer = new Tracer(scene, config);
		for (int i = 0; i < screen->height; ++i) {
			for (int j = 0; j < screen->width; ++j) {
				screen->store(i, j, tracer->trace(Ray(origin, iterator.get(i, j) - origin)));
			}
		}
	}
};
