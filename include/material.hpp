#pragma once

#include "color.hpp"

struct Material {
	Color color;
	float kShading;
	float kReflection;
	float kRefraction;
	float kDiffuseShading;
	float kSpecularShading;
	float index;
};
