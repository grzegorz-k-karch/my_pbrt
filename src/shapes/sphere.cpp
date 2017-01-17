#include "sphere.h"

namespace pbrt {

Bounds3f Sphere::ObjectBound() const {
  return Bounds3f(Point3f(-radius, -radius, zMin),
                  Point3f(radius, radius, zMax));
}

bool Sphere::Intersect(const Ray& r, Float* tHit, SurfaceInteraction* isect,
    bool testAlphaTexture) const {
  Float phi;
  Point3f pHit;

  Vector3f oErr, dErr;
  Ray ray = (*worldToObject)(r, &oErr, &dErr);
  // TODO <compute error bounds for sphere intersection 225>
  EFloat ox(ray.o.x), oy(ray.o.y), oz(ray.o.z); // TODO
  EFloat dx(ray.d.x), dy(ray.d.y), dz(ray.d.z); // TODO

  EFloat a = dx*dx + dy*dy + dz*dz;
  EFloat b = 2*(dx*ox + dy*oy + dz*oz);
  EFloat c = ox*ox + oy*oy + oz*oz - EFloat(radius)*EFloat(radius);

  EFloat t0, t1;
  if (!Quadratic(a, b, c, &t0, &t1))
    return false;

// TODO
//  if (t0.UpperBound() > ray.tMax || t1.LowerBound() <= 0)
//    return false;
//  EFloat tShapeHit = t0;
//  if (tShapeHit.LowerBound() <= 0) {
//    tShapeHit = t1;
//    if (tShapeHit.UpperBound() > ray.tMax) {
//      return false;
//    }
//  }
  if (t0 > ray.tMax || t1 <= 0)
    return false;
  EFloat tShapeHit = t0;
  if (tShapeHit <= 0) {
    tShapeHit = t1;
    if (tShapeHit > ray.tMax) {
      return false;
    }
  }

  // <compute sphere hit position and phi>
  pHit = ray((Float)tShapeHit);
  // TODO <refine sphere intersection point 225>
  if (pHit.x == 0 && pHit.y == 0)
    pHit.x = 1e-5f*radius;
  phi = std::atan2(pHit.y, pHit.x);
  if (phi < 0)
    phi += 2*Pi;

  // <test sphere intersection against clipping parameters>
  if ((zMin > -radius && pHit.z < zMin) || (zMax < radius && pHit.z > zMax)) {
    if (tShapeHit == t1)
      return false;
//    if (t1.UpperBound() > ray.tMax) // TODO
//      return false;
    if (t1 > ray.tMax)
      return false;
    tShapeHit = t1;
    pHit = ray((Float)tShapeHit);
    // TODO <refine sphere intersection point 225>
    if (pHit.x == 0 && pHit.y == 0)
      pHit.x = 1e-5f*radius;
    phi = std::atan2(pHit.y, pHit.x);
    if (phi < 0)
      phi += 2*Pi;
    if ((zMin > -radius && pHit.z < zMin) || (zMax < radius && pHit.z > zMax) || phi > phiMax)
      return false;
  }

  // <find parametric representation of sphere hit>
  Float u = phi/phiMax;
  Float theta = std::acos(Clamp(pHit.z/radius, -1, 1));
  Float v = (theta - thetaMin)/(thetaMax - thetaMin);
  // <compute sphere dpdu and dpdv>
  Float zRadius = std::sqrt(pHit.x*pHit.x + pHit.y*pHit.y);
  Float invZRadius = 1/zRadius;
  Float cosPhi = pHit.x*invZRadius;
  Float sinPhi = pHit.y*invZRadius;
  Vector3f dpdu(-phiMax*pHit.y, phiMax*pHit.x, 0);
  Vector3f dpdv = (thetaMax - thetaMin)*Vector3f(pHit.z*cosPhi, pHit.z*sinPhi, -radius*std::sin(theta));
  // <compute sphere dndu and dndv>

}


} // namespace pbrt

