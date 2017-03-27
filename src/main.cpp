#include <cstdio>
#include "../include/scene.hpp"
#include "../include/camera.hpp"
#include "../include/rectangle_shape.hpp"
#include "../include/area_light.hpp"
#include "../include/color.hpp"

void buildScene(Scene *scene) {
	scene->addObject(new AreaLight(RectangleShape(Vec(3, 1, -3), Vec(5, 0, 0), Vec(5, 0, 1)), Color(1, 1, 1)));
	scene->addObject(new AreaLight(RectangleShape(Vec(3, 1.3, -3), Vec(5, 0.3, 0), Vec(5, 0.3, 1)), Color(1, 1, 1)));
}

int main() {
	freopen("output.ppm", "w", stdout);
	Scene scene;
	buildScene(&scene);
	TraceConfig config;
	Camera camera(Ray(Vec(), Vec(1, 0, 0)), Vec(0, 1, 0), 1.0, 768, 1280, static_cast<float>(2.0 / 1024.0));
	camera.see(&scene, &config);
	camera.screen.print();
	return 0;
}
