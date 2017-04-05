#pragma once

#include <cstdlib>

inline float randf() {
	return rand() / static_cast<float>(RAND_MAX);
}

#include <cmath>
#include <algorithm>

constexpr float EPS = 1e-4;
constexpr float PI = 3.1415926535f;

#ifdef __linux__
namespace std {
	#define powf pow
	#define sinf sin
	#define sqrtf sqrt
	#define cosf cos
	#define expf exp
}
#endif