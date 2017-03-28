#include <cstdio>
#include "../include/renderer.hpp"

int main() {
	FILE *fout = fopen("output.ppm", "w");

	Camera camera(Ray(Vec(), Vec(1, 0, 0)), Vec(0, 1, 0), 1.0, 1.8, 3.2, 720, 1280);
	Renderer *renderer = new Renderer(&camera, buildScene());
	renderer->multiWorkerRender(fout);
	delete(renderer);

	fclose(fout);

	return 0;
}
