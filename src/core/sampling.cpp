#include "sampling.h"

namespace pbrt {

Point2f ConcentricSampleDisk(const Point2f& u) {
  // <map uniform random numbers to [-1,1]^2>
  Point2f uOffset = 2.0f*u - Vector2f(1, 1);

  // <handle degeneracy at the origin>
  if (uOffset.x == 0 && uOffset.y == 0) {
    return Point2f(0, 0);
  }

  // <apply concentric mapping to point>
  Float theta, r;
  if (std::abs(uOffset.x) > std::abs(uOffset.y)) {
    r = uOffset.x;
    theta = PiOver4*(uOffset.y/uOffset.x);
  }
  else {
    r = uOffset.y;
    theta = PiOver2 - PiOver4*(uOffset.x/uOffset.y);
  }

  return r*Point2f(std::cos(theta), std::sin(theta));
}

} // namespace pbrt
