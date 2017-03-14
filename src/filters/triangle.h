#ifndef FILTERS_TRIANGLE_H
#define FILTERS_TRIANGLE_H

#include "pbrt.h"
#include "geometry.h"
#include "filter.h"

namespace pbrt {

class TriangleFilter : public Filter {
public:
  TriangleFilter(const Vector2f& radius)
: Filter(radius) {}

  virtual Float Evaluate(const Point2f& p) const override;
};

} // namespace pbrt

#endif //FILTERS_TRIANGLE_H
