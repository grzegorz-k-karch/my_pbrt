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

void StratifiedSample1D(Float* samp, int nSamples, RNG& rng, bool jitter) {

  Float invNSamples = (Float)1/nSamples;
  for (int i = 0; i < nSamples; ++i) {
    Float delta = jitter ? rng.UniformFloat() : 0.5f;
    samp[i] = std::min((i + delta)*invNSamples, OneMinusEpsilon);
  }
}

void StratifiedSample2D(Point2f* samp, int nx, int ny, RNG& rng, bool jitter) {

  Float dx = (Float)1/nx, dy = (Float)1/ny;
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      Float jx = jitter ? rng.UniformFloat() : 0.5f;
      Float jy = jitter ? rng.UniformFloat() : 0.5f;
      samp->x = std::min((x + jx)*dx, OneMinusEpsilon);
      samp->y = std::min((y + jy)*dy, OneMinusEpsilon);
      samp++;
    }
  }
}

void LatinHypercube(Float* samples, int nSamples, int nDim, RNG& rng) {

  // <generate LHS samples along diagonal>
  Float invNSamples = (Float)1/nSamples;
  for (int i = 0; i < nSamples; ++i) {
    for (int j = 0; j < nDim; ++j) {
      Float sj = (i + (rng.UniformFloat()))*invNSamples;
      samples[nDim*i +j] = std::min(sj, OneMinusEpsilon);
    }
  }

  // <permute LHS samples in each dimension>
  for (int i = 0; i < nDim; ++i) {
    for (int j = 0; j < nSamples; ++j) {
      int other = j + rng.UniformUInt32(nSamples - j);
      std::swap(samples[nDim*j + i], samples[nDim*other + i]);
    }
  }
}

Vector3f UniformSampleHemisphere(const Point2f& sample) {
  Float z = 1 - 2*sample[0];
  Float r = std::sqrt(std::max((Float)0, (Float)1 - z*z));
  Float phi = 2*Pi*sample[1];
  return Vector3f(r*std::cos(phi), r*std::sin(phi), z);
}

Float UniformHemispherePdf() {
  return Inv2Pi;
}

} // namespace pbrt
