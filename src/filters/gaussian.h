#ifndef FILTERS_GAUSSIAN_H
#define FILTERS_GAUSSIAN_H

#include "pbrt.h"
#include "filter.h"
#include "geometry.h"

namespace pbrt {

class GaussianFilter : public Filter {
public:
  GaussianFilter(const Vector2f& radius)
: Filter(radius), alpha(alpha),
  expX(std::exp(-alpha*radius.x*radius.x)),
  expY(std::exp(-alpha*radius.y*radius.y)) {}
  virtual Float Evaluate(const Point2f& p) const override;

private:

  Float Gaussian(Float d, Float expv) const;

  const Float alpha;
  const Float expX, expY;
};

} // namespace pbrt

#endif // FILTERS_GAUSSIAN_H
