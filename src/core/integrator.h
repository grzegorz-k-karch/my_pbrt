#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "scene.h"
#include "camera.h"
#include "sampler.h"
#include "memory.h"
#include <memory>

namespace pbrt {

  class Integrator {

  public:
    // <integrator interface>
    virtual void Render(const Scene& scene) = 0;

  };

  class SamplerIntegrator : public Integrator {

  public:
  SamplerIntegrator(std::shared_ptr<const Camera> camera,
		    std::shared_ptr<Sampler> sampler,
		    const Bounds2i& pixelBounds)
    : camera(camera), sampler(sampler), pixelBounds(pixelBounds) {}

    virtual void Preprocess(const Scene& scene, Sampler& sampler) {}
    virtual void Render(const Scene& scene);
    virtual Spectrum Li(const RayDifferential& ray, const Scene& scene,
			Sampler& sampler, MemoryArena& arena, int depth = 0) const = 0;
    Spectrum SpecularReflect(const RayDifferential& ray, const SurfaceInteraction& isect,
                             const Scene& scene, Sampler& sampler, MemoryArena &arena,
                             int depth) const;
  protected:
    std::shared_ptr<const Camera> camera;
  private:
    std::shared_ptr<Sampler> sampler;
    const Bounds2i pixelBounds;
  };

}
#endif//INTEGRATOR_H
