#include "light.hpp"
#include "color.hpp"
#include "vec.hpp"
#include "rectangle_shape.hpp"

struct AreaLight : Light {
	// members

	RectangleShape rectangle;

	// constructors

	AreaLight(RectangleShape _rectangle, const Color &_color): Light(_color), rectangle(_rectangle) {
	}

	// member methods

	IntersectionResult intersect(const Ray &ray) const {
		return rectangle.intersect(ray);
	}
};
