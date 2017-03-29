#pragma once

#include <cstdio>
#include <vector>
#include "color.hpp"

struct Screen {
	// members

	int height, width;
	std::vector<std::vector<Color>> pixels;

	// constructors

	Screen(int _height, int _width):
			height(_height), width(_width),
			pixels(static_cast<unsigned long>(_height), std::vector<Color>(static_cast<unsigned long>(_width))) {
	}

	// member methods

	Color get(int i, int j) const {
		return pixels[i][j];
	}

	void store(int i, int j, const Color &color) {
		pixels[i][j] = color;
	}

	void print(const char * filename) const {
		FILE *file = fopen(filename, "w");
		fprintf(file, "P3\n");
		fprintf(file, "%d %d\n", width, height);
		fprintf(file, "255\n");
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				Color color = fminf(get(i, j), 1.0) * 255.0;
				fprintf(file, "%d %d %d", int(color.x), int(color.y), int(color.z));
				if (j + 1 == width) {
					fprintf(file, "\n");
				} else {
					fprintf(file, "  ");
				}
			}
		}
		fclose(file);
	}
};
