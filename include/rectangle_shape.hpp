#pragma once

#include "resources.hpp"
#include "finite_planar_shape.hpp"

struct RectangleIterator {
	Vec start, dx, dy;

	Vec get(int i, int j) {
		return start + i * dx + j * dy;
	}
};

struct RectangleShape : FinitePlanarShape {
	// members

	Vec start, height, width;

	// constructors

	RectangleShape(const Vec &a, const Vec &b, const Vec &c): start(b), height(a - b), width(c - b) {
	}

	// member methods

	Vec getBasePoint() const final {
		return start;
	}

	Vec getNormal() const final {
		return width.cross(height).unit();
	}

	bool inside(const Vec &point) const final {
		Vec D = point - start, ND = D - width - height, N = getNormal();
		return width.cross(D).dot(N) >= 0 &&
				D.cross(height).dot(N) >= 0 &&
				height.cross(ND).dot(N) >= 0 &&
				ND.cross(width).dot(N) >= 0;
	}

	RectangleIterator getIterator(int divHeight, int divWidth) const {
		Vec dx = - height / static_cast<float>(divHeight), dy = width / static_cast<float>(divWidth);
		return {.start = start + height - dx / 2.0 + dy / 2.0, .dx = dx, .dy = dy};
	}

	Vec sample() const {
		return start + randf() * height + randf() * width;
	}
};
