#pragma once

#include "color.hpp"

struct Material {
	Color color;
	float kShading;
	float kReflection;
	float kDiffuseShading;
	float kSpecularShading;
};
