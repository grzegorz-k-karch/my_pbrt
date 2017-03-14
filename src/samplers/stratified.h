#ifndef SAMPLERS_STRATIFIED_H
#define SAMPLERS_STRATIFIED_H

#include "pbrt.h"
#include "sampler.h"

namespace pbrt {

class StratifiedSampler : public PixelSampler {
public:
  StratifiedSampler(int xPixelSamples, int yPixelSamples,
      bool jitterSamples, int nSampledDimensions)
: PixelSampler(xPixelSamples*yPixelSamples, nSampledDimensions),
   xPixelSamples(xPixelSamples), yPixelSamples(yPixelSamples),
   jitterSamples(jitterSamples) {}

  virtual void StartPixel(const Point2i& p) override;

private:
  const int xPixelSamples, yPixelSamples;
  const bool jitterSamples;
};

} // namespace pbrt

#endif //SAMPLERS_STRATIFIED_H
