#ifndef INTEGRATORS_WHITTED_H
#define INTEGRATORS_WHITTED_H

#include "integrator.h"
#include "sampler.h"
#include "pbrt.h"
#include <memory>

namespace pbrt {

class WhittedIntegrator : public SamplerIntegrator {

public:
	WhittedIntegrator(int maxDepth, std::shared_ptr<const Camera> camera,
					  std::shared_ptr<Sampler> sampler);
	virtual Spectrum Li(const RayDifferential& ray, const Scene& scene,
	                        Sampler& sampler, MemoryArena& arena, int depth = 0) const;
private:
	const int maxDepth;
};

}

#endif//INTEGRATORS_WHITTED_H
