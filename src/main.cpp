#include <cstdio>
#include "../include/renderer.hpp"

int main() {
	Camera camera(Ray(Vec(0, 1, 0), Vec(1, 0, 0)), Vec(0, 1, 0), 1.0, 1.8, 3.2, 720, 1280);
	Scene scene;
	Renderer *renderer = new Renderer(camera, buildScene(scene));

	FILE *fout = fopen("output.ppm", "w");
	renderer->multiWorkerRender(fout);
	fclose(fout);

	delete(renderer);

	return 0;
}
