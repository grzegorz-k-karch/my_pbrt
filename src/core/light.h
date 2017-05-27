#ifndef LIGHT_H
#define LIGHT_H

#include "pbrt.h"
#include "geometry.h"
#include "transform.h"
#include "medium.h"
#include "interaction.h"

namespace pbrt {

enum class LightFlags : int {
  DeltaPosition = 1, DeltaDirection = 2, Area = 4, Infinite = 8
};

inline bool IsDeltaLight(int flags) {
  return flags & (int)LightFlags::DeltaPosition ||
      flags & (int)LightFlags::DeltaDirection;
}

class VisibilityTester {
public:
  VisibilityTester(const Interaction& p0, const Interaction& p1)
: p0(p0), p1(p1) {}

  const Interaction& P0() const { return p0; }
  const Interaction& P1() const { return p1; }

  bool Unoccluded(const Scene& scene) const;

  Spectrum Tr(const Scene& scene, Sampler& sampler) const;
private:
  Interaction p0, p1;
};

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

  const int flags;
  const int nSamples;
  const MediumInterface mediumInterface;

protected:
  const Transform lightToWorld, worldToLight;
};


class AreaLight : public Light {

};

}

#endif//LIGHT_H
