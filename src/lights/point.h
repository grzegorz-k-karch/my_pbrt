#ifndef LIGHTS_POINT_H_
#define LIGHTS_POINT_H_

#include "pbrt.h"
#include "light.h"
#include "spectrum.h"

namespace pbrt {

class PointLight : public Light {
public:
  PointLight(const Transform& lightToWorld, const MediumInterface& mediumInterface,
      const Spectrum& I)
: Light((int)LightFlags::DeltaPosition, lightToWorld, mediumInterface),
  pLight(lightToWorld(Point3f(0, 0, 0))), I(I) {}

  Spectrum Sample_Li(const Interaction& ref, const Point2f& u, Vector3f*wi,
      Float* pdf, VisibilityTester* vis) const;

  Spectrum Power() const;

private:
  const Point3f pLight;
  const Spectrum I;
};

} /* namespace pbrt */

#endif /* LIGHTS_POINT_H_ */
