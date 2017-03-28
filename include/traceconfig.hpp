#pragma once

struct TraceConfig {
	float lightDecayCoeff = 2.0;
	int areaLightSamples = 50;
	int numWorkers = 16;
};