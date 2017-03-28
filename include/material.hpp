#pragma once

#include "color.hpp"

struct Material {
	Color color;
	float kDiffuseShading;
	float kSpecularShading;
};
