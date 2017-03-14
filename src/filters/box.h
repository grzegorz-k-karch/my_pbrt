#ifndef FILTERS_BOX_H
#define FILTERS_BOX_H

#include "pbrt.h"
#include "filter.h"
#include "geometry.h"

namespace pbrt {

class BoxFilter : public Filter {
public:
  BoxFilter(const Vector2f& radius)
: Filter(radius) {}

  virtual Float Evaluate(const Point2f& p) const override;
};

} // namespace pbrt

#endif // FILTERS_BOX_H
