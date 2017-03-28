#pragma once

#include <cstdlib>

inline float randf() {
	return rand() / static_cast<float>(RAND_MAX);
}
