#ifndef LIGHT_H
#define LIGHT_H

#include "pbrt.h"
#include "geometry.h"
#include "transform.h"
#include "medium.h"

namespace pbrt {

enum class LightFlags : int {
  DeltaPosition = 1, DeltaDirection = 2, Area = 4, Infinite = 8
};

inline bool IsDeltaLight(int flags) {
  return flags & (int)LightFlags::DeltaPosition ||
      flags & (int)LightFlags::DeltaDirection;
}

class Light {

public:
  Light(int flags, const Transform& lightToWorld, const MediumInterface& mediumInterface,
      int nSamples=1)
: flags(flags), nSamples(std::max(1, nSamples)),
  mediumInterface(mediumInterface), lightToWorld(lightToWorld),
  worldToLight(Inverse(lightToWorld)) {
    // <warn if light has transformation with non-uniform scale>
  }

  virtual Spectrum Sample_Li(const Interaction& ref, const Point2f& u, Vector3f*wi,
      Float* pdf, VisibilityTester* vis) const = 0;

  Spectrum Le(const RayDifferential& ray) const;

  virtual Spectrum Power() const = 0;

  virtual void Preprocess(const Scene& scene) {}
protected:
  const Transform lightToWorld, worldToLight;
private:
  const int flags;
  const int nSamples;
  const MediumInterface mediumInterface;
};


class AreaLight : public Light {

};

}

#endif//LIGHT_H
