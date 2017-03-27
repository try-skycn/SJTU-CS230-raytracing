#pragma once

#include <cstdio>
#include <vector>
#include "color.hpp"

struct Screen {
	// members

	std::vector<std::vector<Color>> pixels;
	int height, width;

	// constructors

	Screen(int _height, int _width): height(_height), width(_width), pixels(std::vector<std::vector<Color>>(_height, std::vector<Color>(_width))) {
	}

	// member methods

	Color get(int i, int j) const {
		return pixels[i][j];
	}

	void store(int i, int j, const Color &color) {
		pixels[i][j] = color;
	}

	void print() const {
		printf("P3\n");
		printf("%d %d\n", height, width);
		printf("255\n");
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				Color color = min(get(i, j), 1.0) * 255.0;
				printf("%d %d %d", int(color.x), int(color.y), int(color.z));
				if (j + 1 == width) {
					printf("\n");
				} else {
					printf("  ");
				}
			}
		}
	}
};
