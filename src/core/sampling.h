#ifndef CORE_SAMPLING_H
#define CORE_SAMPLING_H

#include "pbrt.h"
#include "geometry.h"
#include "rng.h"

namespace pbrt {

Point2f ConcentricSampleDisk(const Point2f& u);

void StratifiedSample1D(Float* samp, int nSamples, RNG& rng, bool jitter);
void StratifiedSample2D(Point2f* samp, int nx, int ny, RNG& rng, bool jitter);

template <typename T>
void Shuffle(T* samp, int count, int nDimensions, RNG& rng) {

  for (int i = 0; i < count; ++i) {
    int other = i + rng.UniformUInt32(count - i);
    for (int j = 0; j < nDimensions; ++j) {
      std::swap(samp[nDimensions*i + j], samp[nDimensions*other + j]);
    }
  }
}

void LatinHypercube(Float* samples, int nSamples, int nDim, RNG& rng);

} // namespace pbrt

#endif // CORE_SAMPLING_H
