#ifndef SHAPES_SPHERE_H
#define SHAPES_SPHERE_H

#include "shape.h"
#include "geometry.h"

namespace pbrt {


class Sphere : public Shape {
public:
  Sphere(const Transform* objectToWorld, const Transform* worldToObject,
         bool reverseOrientation, Float radius, Float zMin, Float zMax,
         Float phiMax)
  : Shape(objectToWorld, worldToObject, reverseOrientation), radius(radius),
    zMin(Clamp(std::min(zMin, zMax), -radius, radius)),
    zMax(Clamp(std::max(zMin, zMax), -radius, radius)),
    thetaMin(std::acos(Clamp(zMin/radius, -1, 1))),
    thetaMax(std::acos(Clamp(zMax/radius, -1, 1))),
    phiMax(Radians(Clamp(phiMax, 0, 360))) {}

  Bounds3f ObjectBound() const;

private:
    const Float radius;
    const Float zMin, zMax;
    const Float thetaMin, thetaMax, phiMax;
};

} // namespace pbrt

#endif//SHAPES_SPHERE_H
