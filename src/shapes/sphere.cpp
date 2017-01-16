#include "sphere.h"

namespace pbrt {

Bounds3f Sphere::ObjectBound() const {
  return Bounds3f(Point3f(-radius, -radius, zMin),
                  Point3f(radius, radius, zMax));
}

} // namespace pbrt

