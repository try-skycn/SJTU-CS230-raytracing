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
	RectangleIterator iterator;
	Screen screen;

	// constructors

	Camera(const Ray &sight, const Vec &up, float dist, float height, float width, int divHeight, int divWidth):
			origin(sight.origin), screen(divHeight, divWidth) {
		Vec center = sight.move(dist), right = sight.dir.cross(up);
		RectangleShape rectangle(center + (height * 0.5f) * up - (width * 0.5f) * right,
		                         center - (height * 0.5f) * up - (width * 0.5f) * right,
		                         center - (height * 0.5f) * up + (width * 0.5f) * right
		);
		iterator = rectangle.getIterator(screen.height, screen.width);
	}

	void see(int i, int j, const Scene &scene, const TraceConfig &config) {
		screen.store(i, j, Tracer(scene, config, Ray(origin, iterator.get(i, j) - origin)).trace());
	}
};
