#ifndef FILTERS_SINC_H
#define FILTERS_SINC_H

#include "pbrt.h"
#include "filter.h"
#include "geometry.h"

namespace pbrt {

class LanczosSincFilter : public Filter {
public:
  LanczosSincFilter(const Vector2f& radius, Float tau)
: Filter(radius), tau(tau) {}

  virtual Float Evaluate(const Point2f& p) const override;

  Float Sinc(Float x) const {
    x = std::abs(x);
    if (x < 1e-5) return 1;
    return std::sin(Pi*x)/(Pi*x);
  }

  Float WindowedSinc(Float x, Float radius) const {
    x = std::abs(x);
    if (x > radius) return 0;
    Float lanczos = Sinc(x/tau);
    return Sinc(x)*lanczos;
  }

private:
  const Float tau;
};

} // namespace pbrt

#endif // FILTERS_SINC_H
