#ifndef CORE_INTERACTION_H
#define CORE_INTERACTION_H

#include "geometry.h"
#include "medium.h"
#include "pbrt.h"
#include "primitive.h"

namespace pbrt {

struct Interaction {

  Interaction() : time(0) {}
  Interaction(const Point3f& p, const Normal3f& n, const Vector3f& pError,
      const Vector3f& wo, Float time) // TODO, const MediumInterface& mediumInterface)
  : p(p), time(time), pError(pError), wo(wo), n(n) {} // TODO , mediumInterface(mediumInterface) {}

  bool IsSurfaceInteraction() const {
    return n != Normal3f();
  }

  Ray SpawnRay(const Vector3f& d) const {
    Point3f o = OffsetRayOrigin(p, pError, n, d);
    return Ray(o, d, Infinity, time);//, GetMedium(d)); // TODO
  }
  Ray SpawnRayTo(const Point3f& p2) const {
    Point3f origin = OffsetRayOrigin(p, pError, n, p2 - p);
    Vector3f d = p2 - origin;
    return Ray(origin, d, 1 - ShadowEpsilon, time);//, GetMedium(d)); // TODO
  }

  // data
  Point3f p;
  Float time;
  Vector3f pError;
  Vector3f wo;
  Normal3f n;
  // TODO MediumInterface mediumInterface;
};

class SurfaceInteraction : public Interaction {

public:
  SurfaceInteraction() {}
  SurfaceInteraction(const Point3f& p, const Vector3f& pError, const Point2f& uv,
                     const Vector3f& wo, const Vector3f& dpdu, const Vector3f& dpdv,
                     const Normal3f& dndu, const Normal3f& dndv, Float time,
                     const Shape* shape);
  
  void SetShadingGeometry(const Vector3f& dpdus, const Vector3f& dpdvs,
                          const Normal3f& dndus, const Normal3f& dndvs,
                          bool orientationIsAuthoritative);

  // data
  Point2f uv;
  Vector3f dpdu, dpdv;
  Normal3f dndu, dndv;
  const Shape *shape = nullptr;

  struct {
    Normal3f n;
    Vector3f dpdu, dpdv;
    Normal3f dndu, dndv;
  } shading;

  const Primitive *primitive = nullptr;
// TODO BSDF *bsdf = nullptr;
// TODO  BSSRDF *bssrdf = nullptr;
};

} // namespace pbrt

#endif//CORE_INTERACTION_H
