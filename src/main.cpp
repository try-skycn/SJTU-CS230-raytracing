#include <cstdio>
#include "../include/renderer.hpp"

int main() {
	srand(static_cast<unsigned int>(time(0)));
	Camera camera(Ray(Vec(0, 1, 0), Vec(1, 0, 0)), Vec(0, 1, 0), 1.0, 1.8, 3.2, 720, 1280);
	Scene scene;
	Renderer *renderer = new Renderer(camera, buildScene(scene));

	renderer->multiWorkerRender("output.ppm");

	delete(renderer);

	return 0;
}
