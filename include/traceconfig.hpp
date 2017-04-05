#pragma once

struct TraceConfig {
	float lightDecayCoeff = 2.0;
	int diffuseShadingNumbers = 500;
	int numWorkers = 16;
};