#pragma once

#include <atomic>
#include <chrono>
#include <limits>
#include <vector>
#include <thread>
#include <tuple>
#include <cstdint>
#include <cassert>
#include "../vender/concurrentqueue/concurrentqueue.h"

#include "scene.hpp"
#include "screen.hpp"
#include "tracer.hpp"
#include "rectangle_object.hpp"
#include "camera.hpp"
#include "traceconfig.hpp"

struct Renderer {
	// members
	std::atomic<int> cnt_rendered;
	std::atomic<bool> flag_to_stop;
	std::atomic<bool> flag_stopped;

	Camera *camera;
	Scene *scene;
	TraceConfig config;

	// constructors

	Renderer(Camera *_camera, Scene *_scene): camera(_camera), scene(_scene) {
	}

	// member methods

	bool render(FILE *file) {
		for (int i = 0; i < camera->screen.height; ++i) {
			for (int j = 0; j < camera->screen.width; ++j) {
				camera->see(i, j, scene, &config);
			}
		}
		camera->screen.print(file);
		return true;
	}

	bool multiWorkerRender(FILE *file) {
		flag_to_stop = false;
		flag_stopped = false;
		cnt_rendered = 0;

		moodycamel::ConcurrentQueue<std::pair<int, int>> q;
		auto func = [&] {
			for (std::pair<int, int> item; ;) {
				q.try_dequeue(item);
				int i = item.first, j = item.second;
				if (i == -1) return;

				camera->see(i, j, scene, &config);
				++cnt_rendered;
			}
		};

		auto start = std::chrono::high_resolution_clock::now();
		std::vector<std::pair<int, int>> indices;
		for (int i = 0; i < camera->screen.height; ++i) {
			for (int j = 0; j < camera->screen.width; ++j) {
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
			std::this_thread::sleep_for(std::chrono::milliseconds(50));

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
		flag_stopped = true;
		return true;
	}

	void stop() {
		flag_to_stop = true;
	}
};

Scene *buildScene() {
	Scene *scene = new Scene;
	scene->addObject(
			new AreaLight(
					RectangleShape(Vec(10, 2, -2), Vec(5, 2, -2), Vec(5, 2, 2)),
					Color(1, 1, 1)
			)
	);
//	scene->addObject(new AreaLight(RectangleShape(Vec(3, 1.3, -3), Vec(5, 0.3, 0), Vec(5, 0.3, 1)), Color(1, 1, 1)));
//	scene->addObject(new SpotLight(Vec(4, 2, 0), Color(1, 1, 1)));
//	scene->addObject(new SpotLight(Vec(4, 2, -1), Color(1, 1, 1)));
//	scene->addObject(new SpotLight(Vec(4, 2, 1), Color(1, 1, 1)));
	scene->addObject(
			new RectangleObject(
					RectangleShape(Vec(5, -2, -2), Vec(3, -2, -2), Vec(3, -2, 2)),
					Material{
							.color = Color(1, 1, 1),
							.kDiffuseShading = 0.2,
							.kSpecularShading = 0.8
					}
			)
	);
	scene->addObject(
			new RectangleObject(
					RectangleShape(Vec(4.3f, -1.5f, -0.3f), Vec(3.7f, -1.5f, -0.3f), Vec(3.7f, -1.5f, 0.3f)),
					Material{
							.color = Color(0, 0, 1),
							.kDiffuseShading = 0.2,
							.kSpecularShading = 0.8
					}
			)
	);
	return scene;
}
