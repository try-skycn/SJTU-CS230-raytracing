#pragma once

#include "object.hpp"
#include "color.hpp"

struct Light : Object {
	// members

	Color color;

	// member methods

	Light(const Color &_color): color(_color) {
	}
};
