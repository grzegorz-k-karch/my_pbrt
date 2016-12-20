#include "sampler.h"
#include "geometry.h"

namespace pbrt {

void Sampler::StartPixel(const Point2i& p) {
	currentPixel = p;
	currentPixelSampleIndex = 0;
	// ... reset array offsets for next pixel sample
}

bool Sampler::StartNextSample() {
	// ... reset array offsets for next pixel sample
	return ++currentPixelSampleIndex < samplesPerPixel;
}

}
