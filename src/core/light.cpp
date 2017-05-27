#include "light.h"
#include "spectrum.h"
#include "scene.h"


namespace pbrt {

bool VisibilityTester::Unoccluded(const Scene& scene) const {

  return !scene.IntersectP(p0.SpawnRayTo(p1));
}

Spectrum VisibilityTester::Tr(const Scene& scene, Sampler& sampler) const {

  Ray ray(p0.SpawnRayTo(p1));
  Spectrum Tr(1.f);
  // TODO
}


Spectrum Light::Le(const RayDifferential& ray) const {
    return Spectrum(0.0f);
}


}
