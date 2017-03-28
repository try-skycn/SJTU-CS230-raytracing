#include <cstdio>
#include "../include/scene.hpp"
#include "../include/camera.hpp"
#include "../include/rectangle_shape.hpp"
#include "../include/area_light.hpp"
#include "../include/color.hpp"
#include "../include/rectangle_object.hpp"
#include "../include/material.hpp"
#include "../include/spot_light.hpp"

void buildScene(Scene *scene) {
//	scene->addObject(new AreaLight(RectangleShape(Vec(5, 2, -2), Vec(3, 2, -2), Vec(3, 2, 2)), Color(1, 1, 1)));
//	scene->addObject(new AreaLight(RectangleShape(Vec(3, 1.3, -3), Vec(5, 0.3, 0), Vec(5, 0.3, 1)), Color(1, 1, 1)));
	scene->addObject(new SpotLight(Vec(4, 2, 0), Color(1, 1, 1)));
	scene->addObject(new SpotLight(Vec(4, 2, -1), Color(1, 1, 1)));
	scene->addObject(new SpotLight(Vec(4, 2, 1), Color(1, 1, 1)));
	scene->addObject(new RectangleObject(RectangleShape(Vec(5, -2, -2), Vec(3, -2, -2), Vec(3, -2, 2)), Material{.color = Color(1, 1, 1)}));
	scene->addObject(new RectangleObject(RectangleShape(Vec(4.3f, -1.5f, -0.3f), Vec(3.7f, -1.5f, -0.3f), Vec(3.7f, -1.5f, 0.3f)), Material{.color = Color(0, 0, 1)}));
}

int main() {
	freopen("output.ppm", "w", stdout);
	Scene scene;
	buildScene(&scene);
	TraceConfig config;
	Screen screen(720, 1280);
	Camera camera(Ray(Vec(), Vec(1, 0, 0)), Vec(0, 1, 0), 1.0, 1.8, 3.2);
	camera.see(&screen, &scene, &config);
	screen.print();
	return 0;
}
