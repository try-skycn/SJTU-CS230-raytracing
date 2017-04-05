#pragma once

struct TraceConfig {
	float lightDecayCoeff = 2.0;
	int diffuseShadingNumbers = 30;
	int numWorkers = 16;
};