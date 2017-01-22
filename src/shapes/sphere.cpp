#include "sphere.h"
#include "efloat.h"

namespace pbrt {

Bounds3f Sphere::ObjectBound() const {
  return Bounds3f(Point3f(-radius, -radius, zMin),
                  Point3f(radius, radius, zMax));
}

bool Sphere::Intersect(const Ray& r, Float* tHit, SurfaceInteraction* isect,
    bool testAlphaTexture) const {
  Float phi;
  Point3f pHit;

  // <transform Ray to object space
  Vector3f oErr, dErr;
  Ray ray = (*worldToObject)(r, &oErr, &dErr);
  // <compute error bounds for sphere intersection>
  EFloat ox(ray.o.x, oErr.x), oy(ray.o.y, oErr.y), oz(ray.o.z, oErr.z);
  EFloat dx(ray.d.x, dErr.x), dy(ray.d.y, dErr.y), dz(ray.d.z, dErr.z);

  // <compute quadratic sphere coefficients>
  EFloat a = dx*dx + dy*dy + dz*dz;
  EFloat b = 2*(dx*ox + dy*oy + dz*oz);
  EFloat c = ox*ox + oy*oy + oz*oz - EFloat(radius)*EFloat(radius);

  // <solve quadratic equation for t values>
  EFloat t0, t1;
  if (!Quadratic(a, b, c, &t0, &t1))
    return false;

  // <check quadratic shape t0 and t1 for nearest intersection>
  if (t0.UpperBound() > ray.tMax || t1.LowerBound() <= 0)
    return false;
  EFloat tShapeHit = t0;
  if (tShapeHit.LowerBound() <= 0) {
    tShapeHit = t1;
    if (tShapeHit.UpperBound() > ray.tMax) {
      return false;
    }
  }

  // <compute sphere hit position and phi>
  pHit = ray((Float)tShapeHit);

  // <refine sphere intersection point 225>
  pHit *= radius/Distance(pHit, Point3f(0,0,0));

  if (pHit.x == 0 && pHit.y == 0)
    pHit.x = 1e-5f*radius;
  phi = std::atan2(pHit.y, pHit.x);
  if (phi < 0)
    phi += 2*Pi;

  // <test sphere intersection against clipping parameters>
  if ((zMin > -radius && pHit.z < zMin) || (zMax < radius && pHit.z > zMax)) {
    if (tShapeHit == t1)
      return false;
    if (t1.UpperBound() > ray.tMax)
      return false;
    tShapeHit = t1;
    pHit = ray((Float)tShapeHit);

    // <refine sphere intersection point 225>
    pHit *= radius/Distance(pHit, Point3f(0,0,0));

    if (pHit.x == 0 && pHit.y == 0) {
      pHit.x = 1e-5f*radius;
    }
    phi = std::atan2(pHit.y, pHit.x);
    if (phi < 0) {
      phi += 2*Pi;
    }
    if ((zMin > -radius && pHit.z < zMin) ||
        (zMax < radius && pHit.z > zMax) || phi > phiMax) {
      return false;
    }
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
  Vector3f d2Pduu = -phiMax*phiMax*Vector3f(pHit.x, pHit.y, 0);
  Vector3f d2Pduv = (thetaMax - thetaMin)*pHit.z*phiMax*Vector3f(-sinPhi, cosPhi, 0);
  Vector3f d2Pdvv = -(thetaMax - thetaMin)*(thetaMax - thetaMin)*Vector3f(pHit.x, pHit.y, pHit.z);

  // <compute coefficients for fundamental forms>
  Float E = Dot(dpdu, dpdu);
  Float F = Dot(dpdu, dpdv);
  Float G = Dot(dpdv, dpdv);
  Vector3f N = Normalize(Cross(dpdu, dpdv));
  Float e = Dot(N, d2Pduu);
  Float f = Dot(N, d2Pduv);
  Float g = Dot(N, d2Pdvv);

  // <compute sphere dndu and dndv from fundamental form coefficients>
  Float invEGF2 = 1/(E*G - F*F);
  Normal3f dndu = Normal3f((f*F - e*G)*invEGF2*dpdu +
                           (e*F - f*E)*invEGF2*dpdv);
  Normal3f dndv = Normal3f((g*F - f*G)*invEGF2*dpdu +
                           (f*F - g*E)*invEGF2*dpdv);

  // <compute error bounds for sphere intersection>
  Vector3f pError = gamma(5)*Abs((Vector3f)pHit);
  // <initialize SurfaceInteraction from parametric information>
  *isect = (*objectToWorld)(SurfaceInteraction(pHit, pError, Point2f(u, v),
                                               -ray.d, dpdu, dpdv, dndu, dndv, ray.time, this));
  *tHit = (Float)tShapeHit;
}

bool Sphere::IntersectP(const Ray& r, bool testAlphaTexture) const {

  Float phi;
  Point3f pHit;

  // <transform Ray to object space
  Vector3f oErr, dErr;
  Ray ray = (*worldToObject)(r, &oErr, &dErr);

  // TODO <compute error bounds for sphere intersection 225>
  EFloat ox(ray.o.x), oy(ray.o.y), oz(ray.o.z); // TODO
  EFloat dx(ray.d.x), dy(ray.d.y), dz(ray.d.z); // TODO

  // <compute quadratic sphere coefficients>
  EFloat a = dx*dx + dy*dy + dz*dz;
  EFloat b = 2*(dx*ox + dy*oy + dz*oz);
  EFloat c = ox*ox + oy*oy + oz*oz - EFloat(radius)*EFloat(radius);

  // <solve quadratic equation for t values>
  EFloat t0, t1;
  if (!Quadratic(a, b, c, &t0, &t1))
    return false;

  // <check quadratic shape t0 and t1 for nearest intersection>
  if (t0.UpperBound() > ray.tMax || t1.LowerBound() <= 0)
    return false;
  EFloat tShapeHit = t0;
  if (tShapeHit.LowerBound() <= 0) {
    tShapeHit = t1;
    if (tShapeHit.UpperBound() > ray.tMax) {
      return false;
    }
  }

  // <compute sphere hit position and phi>
  pHit = ray((Float)tShapeHit);
  // <refine sphere intersection point 225>
  pHit *= radius/Distance(pHit, Point3f(0,0,0));

  if (pHit.x == 0 && pHit.y == 0)
    pHit.x = 1e-5f*radius;
  phi = std::atan2(pHit.y, pHit.x);
  if (phi < 0)
    phi += 2*Pi;

  // <test sphere intersection against clipping parameters>
  if ((zMin > -radius && pHit.z < zMin) || (zMax < radius && pHit.z > zMax)) {
    if (tShapeHit == t1)
      return false;
    if (t1.UpperBound() > ray.tMax)
      return false;
    tShapeHit = t1;
    pHit = ray((Float)tShapeHit);
    // <refine sphere intersection point 225>
    pHit *= radius/Distance(pHit, Point3f(0,0,0));

    if (pHit.x == 0 && pHit.y == 0)
      pHit.x = 1e-5f*radius;
    phi = std::atan2(pHit.y, pHit.x);
    if (phi < 0)
      phi += 2*Pi;
    if ((zMin > -radius && pHit.z < zMin) || (zMax < radius && pHit.z > zMax) || phi > phiMax)
      return false;
  }
  return true;
}

Float Sphere::Area() const {
  return phiMax*radius*(zMax - zMin);
}

} // namespace pbrt

