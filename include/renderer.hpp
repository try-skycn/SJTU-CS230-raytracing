#pragma once

#include <atomic>
#include <chrono>
#include <limits>
#include <vector>
#include <thread>
#include <tuple>
#include <cstdint>
#include <cassert>
#include <cstring>
#include "../vender/concurrentqueue/concurrentqueue.h"

#include "resources.hpp"
#include "scene.hpp"
#include "screen.hpp"
#include "tracer.hpp"
#include "geometry_object.hpp"
#include "plane_shape.hpp"
#include "sphere_shape.hpp"
#include "triangle_shape.hpp"
#include "camera.hpp"
#include "traceconfig.hpp"

struct Renderer {
	// members
	std::atomic<int> cnt_rendered;
	std::atomic<bool> flag_to_stop;
	std::atomic<bool> flag_stopped;

	Camera &camera;
	const Scene &scene;
	const TraceConfig &config;

	// constructors

	Renderer(Camera &_camera, const Scene &_scene, const TraceConfig &_config): camera(_camera), scene(_scene), config(_config) {
	}

	// member methods

	bool render(const char *filename) {
		for (int i = 0; i < camera.screen.height; ++i) {
			for (int j = 0; j < camera.screen.width; ++j) {
				camera.see(i, j, scene, config);
			}
		}
		camera.screen.print(filename);
		return true;
	}

	bool multiWorkerRender(const char *filename) {
		// referenced to https://github.com/abcdabcd987/ray-tracing/blob/master/src/raytracer.hpp
		flag_to_stop = false;
		flag_stopped = false;
		cnt_rendered = 0;

		moodycamel::ConcurrentQueue<std::pair<int, int>> q;
		auto func = [&] {
			for (std::pair<int, int> item; ;) {
				q.try_dequeue(item);
				int i = item.first, j = item.second;
				if (i == -1) return;

				camera.see(i, j, scene, config);
				++cnt_rendered;
			}
		};

		auto start = std::chrono::high_resolution_clock::now();
		std::vector<std::pair<int, int>> indices;
		for (int i = 0; i < camera.screen.height; ++i) {
			for (int j = 0; j < camera.screen.width; ++j) {
				indices.emplace_back(i, j);
			}
		}
		std::random_shuffle(indices.begin(), indices.end());
		for (const auto &ind : indices) q.enqueue(ind);

		std::vector<std::thread> workers;
		for (int i = 0; i < config.numWorkers; ++i) workers.emplace_back(func);
		for (int i = 0; i < config.numWorkers; ++i) q.enqueue(std::make_pair(-1, -1));
		const int total = indices.size();
		for (;;) {
			int cnt = cnt_rendered.load();
			auto now = std::chrono::high_resolution_clock::now();
			double sec = (now - start).count() / 1e9;
			fprintf(stderr, "\rrendered %d/%d pixels using %d workers in %.3fs...", cnt, total, config.numWorkers, sec);
			if (cnt == total) break;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));

			// if force stop
			if (flag_to_stop) {
				fprintf(stderr, "got stop flag..."); fflush(stderr);
				std::pair<int, int> ignore;
				while (q.try_dequeue(ignore));
				for (int i = 0; i < config.numWorkers; ++i) q.enqueue(std::make_pair(-1, -1));
				for (auto &worker : workers) worker.join();
				fprintf(stderr, "stopped\n");
				flag_stopped = true;
				return false;
			}
		}
		for (auto &worker : workers) worker.join();
		fprintf(stderr, "done\n");
		camera.screen.print(filename);
		flag_stopped = true;
		return true;
	}

	void stop() {
		flag_to_stop = true;
	}
};

void loadObjFile(const char *filename, Scene &scene, float rotate, float stretch, const Vec &move) {
	// referenced to https://github.com/abcdabcd987/ray-tracing/blob/master/src/geometry.hpp
	FILE *f = fopen(filename, "r");
	if (!f) return;

	Material defaultMaterial{
			.color = Color(0, 1, 1),
			.kShading = 1.0f,
			.kReflection = 0.0f,
			.kDiffuseReflection = 0.0f,
			.kRefraction = 0.0f,
			.kDiffuseShading = 0.7f,
			.kSpecularShading = 0.3f,
			.index = 1.0f
	};

	std::vector<Vec> vertices;
	char buf[100];
	while (fscanf(f, " %s", buf) != EOF) {
		if (strcmp(buf, "v") == 0) {
			float x, y, z;
			fscanf(f, "%f%f%f", &x, &y, &z);
			Vec point = Vec(x * cosf(rotate) - z * sinf(rotate), y, x * sinf(rotate) + z * cosf(rotate)) * stretch + move;
			vertices.push_back(point);
		} else if (strcmp(buf, "f") == 0) {
			int v[3];
			for (int i = 0; i < 3; ++i) {
				fscanf(f, " %s", buf);
				sscanf(buf, "%d", &v[i]);
				--v[i];
			}
			scene.addObject(new GeometryObject(new TriangleShape(vertices[v[0]], vertices[v[1]], vertices[v[2]]), defaultMaterial));
		} else if (buf[0] == '#'
		           || strcmp(buf, "mtllib") == 0
		           || strcmp(buf, "vn") == 0
		           || strcmp(buf, "vt") == 0
		           || strcmp(buf, "s") == 0
		           || strcmp(buf, "g") == 0
		           || strcmp(buf, "o") == 0
		           || strcmp(buf, "usemtl") == 0) {
			// not supported
			while (fgetc(f) != '\n');
		} else {
			return;
		}
	}
	fclose(f);
}

const Scene &buildScene(Scene &scene) {
	scene.addObject(
			new AreaLight(
					RectangleShape(Vec(2.2f, 2.5f, -0.2f), Vec(1.8f, 2.5f, -0.2f), Vec(1.8f, 2.5f, 0.2f)),
					Color(1, 1, 1), 50
			)
	);

//	scene.addObject(new SpotLight(Vec(2, 3, 0), Color(1, 1, 1)));
//	scene->addObject(new SpotLight(Vec(2, 1, 1), Color(1, 1, 1)));
//	scene->addObject(new SpotLight(Vec(1.5, 0.5, -1.75f), Color(1, 1, 1) * 0.25));
	Material wallMaterial{
			.color = Color(1, 1, 1),
			.kShading = 1.0f,
			.kReflection = 0.0f,
			.kDiffuseReflection = 0.0f,
			.kRefraction = 0.0f,
			.kDiffuseShading = 1.0f,
			.kSpecularShading = 0.0f,
			.index = 1.0f
	};
	GeometryObject *newObject;
	// bottom
	newObject = new GeometryObject(new PlaneShape(Vec(0, 0, 0), Vec(0, 1, 0)), wallMaterial);
	newObject->material.color = Color(1, 1, 1) * 0.7f;
	newObject->material.kShading = 0.6f;
	newObject->material.kDiffuseReflection = 0.4f;
	scene.addObject(newObject);
	// left
	newObject = new GeometryObject(new PlaneShape(Vec(0, 0, -3), Vec(0, 0, 1)), wallMaterial);
	newObject->material.color = Color(0, 0.8, 0);
	scene.addObject(newObject);
//	// right
	newObject = new GeometryObject(new PlaneShape(Vec(0, 0, 3), Vec(0, 0, -1)), wallMaterial);
	newObject->material.color = Color(0, 0, 0.8);
	scene.addObject(newObject);
	// top
//	newObject = new GeometryObject(new PlaneShape(Vec(0, 4, 0), Vec(0, -1, 0)), wallMaterial);
//	newObject->material.color = Color(0.8, 0, 0.8);
//	scene.addObject(newObject);
	// front
	newObject = new GeometryObject(new PlaneShape(Vec(5, 0, 0), Vec(-1, 0, 0)), wallMaterial);
	newObject->material.color = Color(0.8f, 0.4, 1);
	newObject->material.kShading = 0.6f;
	newObject->material.kReflection = 0.4f;
	scene.addObject(newObject);
	// ball
	scene.addObject(
			new GeometryObject(
					new SphereShape(Vec(2, 0.5f, 0), 0.5f),
					Material{
							.color = Color(0, 1, 0),
							.kShading = 0.5f,
							.kReflection = 0.0f,
							.kDiffuseReflection = 0.5f,
							.kRefraction = 0.7f,
							.kDiffuseShading = 0.5f,
							.kSpecularShading = 0.5f,
							.index = 1.1
					}
			)
	);
//	scene.addObject(
//			new GeometryObject(
//					new SphereShape(Vec(3, 0.3f, 0), 0.3f),
//					Material{
//							.color = Color(0, 1, 0),
//							.kShading = 1.0f,
//							.kReflection = 0.0f,
//							.kDiffuseReflection = 0.0f,
//							.kRefraction = 0.0f,
//							.kDiffuseShading = 0.5f,
//							.kSpecularShading = 0.5f,
//							.index = 1.1
//					}
//			)
//	);
	// triangle
//	newObject = new GeometryObject(new TriangleShape(Vec(3, 1, 0), Vec(2, -0.5f, -1.0f), Vec(1.5f, -1.0f, 1.0f)), wallMaterial);
//	newObject->material.color = Vec(0, 1.0f, 1.0f);
//	scene.addObject(newObject);
	// obj files
//	loadObjFile("../models/teapot.obj", scene, PI * 0.5f, 0.25f, Vec(2, 0.5f, 0));
	return scene;
}
