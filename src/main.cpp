#include "../include/scene.hpp"
#include "../include/camera.hpp"

int main() {
	Scene scene;
	TraceConfig config;
	Camera camera(Ray(Vec(), Vec(1, 0, 0)), Vec(0, 1, 0), 1.0, 1024, 768, static_cast<float>(2.0 / 1024.0));
	camera.see(&scene, &config);
	return 0;
}
