#pragma once

#include <vector>
#include "vec.hpp"

struct Object {
	static Vec get_reflected(const Vec &in_dir, const Vec &normal) { return (in_dir - 2.0 * in_dir.dot(normal) * normal).unit(); }
	static Vec get_refracted(const Vec &in_dir, const Vec &normal, float in_index, float out_index) { assert(false); return Vec(); }
};

struct Scene {
	vector<Object> objects;

};