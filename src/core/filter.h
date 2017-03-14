#ifndef CORE_FILTER_H
#define CORE_FILTER_H

#include "pbrt.h"
#include "geometry.h"

namespace pbrt {

class Filter {
public:
  Filter(const Vector2f& radius)
: radius(radius), invRadius(Vector2f(1/radius.x, 1/radius.y)) {}

  virtual Float Evaluate(const Point2f& p) const = 0;

  const Vector2f radius, invRadius;
};

} // namespace pbrt

#endif // CORE_FILTER_H
