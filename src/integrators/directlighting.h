#ifndef INTEGRATORS_DIRECT_LIGHTING_H
#define INTEGRATORS_DIRECT_LIGHTING_H

#include "pbrt.h"
#include "integrator.h"
#include "scene.h"

#include <memory>

namespace pbrt {

  enum class LightStrategy { UniformSampleAll, UniformSampleOne };

  class DirectLightingIntegrator : public SamplerIntegrator {

  public:
    DirectLightingIntegrator(LightStrategy strategy, int maxDepth,
			     std::shared_ptr<const Camera> camera,
			     std::shared_ptr<Sampler> sampler,
			     const Bounds2i& pixelBounds)
      : SamplerIntegrator(camera, sampler, pixelBounds),
      strategy(strategy), maxDepth(maxDepth) {}
    
    virtual Spectrum Li(const RayDifferential& ray, const Scene& scene,
			Sampler& sampler, MemoryArena& arena, int depth = 0) const override;

  private:
    const LightStrategy strategy;
    const int maxDepth;
    std::vector<int> nLightSamples;
  };

} // namespace pbrt

#endif//INTEGRATORS_DIRECT_LIGHTING_H
