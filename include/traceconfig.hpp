#pragma once

struct TraceConfig {
	float lightDecayCoeff = 3.0;
	int areaLightSamples = 10;
	int numWorkers = 8;
};