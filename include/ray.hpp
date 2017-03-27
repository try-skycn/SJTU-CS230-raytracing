#include "vec.hpp"

struct Ray {
	// members

	Vec origin, direction;

	// constructors

	Ray(const Vec &_origin, const Vec &_direction): origin(_origin), direction(_direction.unit()) {
		origin += EPS * direction;
	}
};