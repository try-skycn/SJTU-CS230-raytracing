#pragma once

struct TraceConfig {
	float lightDecayCoeff = 2.0;
	int diffuseShadingNumbers = 1;
	int numWorkers = 32;
};