#include "directlighting.h"

namespace pbrt {

  Spectrum DirectLightingIntegrator::Li(const RayDifferential& ray, const Scene& scene,
					Sampler& sampler, MemoryArena& arena, int depth) const {

    Spectrum L(0.f);
    
    // Find closest ray intersection or return background radiance
    SurfaceInteraction isect;
    if (!scene.Intersect(ray, &isect)) {
      for (const auto &light : scene.lights) {
	L += light->Le(ray);
      }
      return L;
    }
    
    // Compute scattering functions for surface interaction
    isect.ComputeScatteringFunctions(ray, arena);
    if (!isect.bsdf) {
      return Li(isect.SpawnRay(ray.d), scene, sampler, arena, depth);
    }

    Vector3f wo = isect.wo;
    // Compute emitted light if ray hit an area light source
    L += isect.Le(wo);

    if (scene.lights.size() > 0) {
      // TODO [2017-06-01]
    }
  }

} // namespace pbrt
