#include "point.h"

namespace pbrt {

Spectrum PointLight::Sample_Li(const Interaction& ref, const Point2f& u, Vector3f*wi,
      Float* pdf, VisibilityTester* vis) const {

  *wi = Normalize(pLight - ref.p);
  *pdf = 1.f;
  *vis = VisibilityTester(ref, Interaction(pLight, ref.time, mediumInterface));

  return I/DistanceSquared(pLight, ref.p);
}

Spectrum PointLight::Power() const {
  return 4*Pi*I;
}
} /* namespace pbrt */
