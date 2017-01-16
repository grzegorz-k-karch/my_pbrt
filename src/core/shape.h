#ifndef CORE_SHAPE_H
#define CORE_SHAPE_H

#include "interaction.h"
#include "transform.h"
#include "geometry.h"

namespace pbrt {

class Transform;

class Shape {

public:
  Shape(const Transform* objectToWorld,
      const Transform* worldToObject,
      bool reverseOrientation);

  virtual ~Shape() {};
  virtual Bounds3f ObjectBound() const = 0;
  virtual Bounds3f WorldBound() const;

  virtual bool Intersect(const Ray& ray, Float* tHit, SurfaceInteraction* isect,
      bool testAlphaTexture = true) const = 0;
  virtual bool IntersetP(const Ray& ray, bool testAlphaTexture = true) const {
    Float tHit = ray.tMax;
    SurfaceInteraction isect;
    return Intersect(ray, &tHit, &isect, testAlphaTexture);
  }

  virtual Float Area() const = 0;

  // data
  const Transform *objectToWorld, *worldToObject;
  const bool reverseOrientation;
  const bool transformSwapsHandedness;
};

} // namespace pbrt

#endif //CORE_SHAPE_H
