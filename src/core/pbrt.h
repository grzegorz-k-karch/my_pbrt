#ifndef PBRT_H
#define PBRT_H

#include <assert.h>
#include <limits>
#include <cmath>
#include <utility>
#include <cstdint>
#include <cstring>

namespace pbrt {

class Aggregate;
class AnimatedTransform;
class AreaLight;
class AtomicFloat;
class CameraSample;
class Film;
class Filter;
class Material;
class Medium;
class MediumInterface;
class MemoryArena;
class Primitive;
class PixelSampler;
class Quaternion;
class Ray;
class Shape;
class SurfaceInteraction;
class Transform;
class RGBSpectrum;
class RNG;
class SampledSpectrum;
typedef RGBSpectrum Spectrum;
//typedef SampledSpectrum Spectrum;

#ifdef PBRT_FLOAT_AS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif//PBRT_FLOAT_AS_DOUBLE

#define ALLOCA(TYPE, COUNT) (TYPE*)alloca((COUNT)*sizeof(TYPE))

#ifdef NDEBUG
#define Assert(expr) ((void)0)
#else
#define Assert(expr) assert(expr) // gkk
//#define Assert(expr) \
//    ((expr) ? (void)0 : \
//            Severe("Assertion \"%s\" failed in %s. line %d", \
//                    #expr, __FILE__, __LINE__))
#endif//NDEBUG

// <global constants>
static constexpr Float MaxFloat = std::numeric_limits<Float>::max();
static constexpr Float Infinity = std::numeric_limits<Float>::infinity();
static constexpr Float MachineEpsilon = std::numeric_limits<Float>::epsilon() * 0.5;

const Float ShadowEpsilon = 0.0001f;

static constexpr Float Pi =      3.14159265358979323846;
static constexpr Float InvPi =   0.31830988618379067154;
static constexpr Float PiOver2 = 1.57079632679489655800;
static constexpr Float PiOver4 = 0.78539816339744827900;



// <global inline functions>
inline Float Radians(Float deg) { return (Pi/180)*deg; }
inline Float Degrees(Float rad) { return (180/Pi)*rad; }

inline Float Lerp(Float t, Float v1, Float v2) {
  return (1 - t)*v1 + t*v2;
}

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
  if (val < low)
    return low;
  else if (val > high)
    return high;
  else
    return val;
}

inline bool Quadratic(Float a, Float b, Float c, Float* t0, Float* t1) {
  double discrim = (double)b*(double)b - 4*(double)a*(double)c;
  if (discrim < 0)
    return false;
  double rootDiscrim = std::sqrt(discrim);
  double q;
  if (b < 0)
    q = -0.5*(b - rootDiscrim);
  else
    q = -0.5*(b + rootDiscrim);
  *t0 = q/a;
  *t1 = c/q;
  if (*t0 > *t1)
    std::swap(*t0, *t1);
  return true;
}

inline uint32_t FloatToBits(float f) {
  uint32_t ui;
  memcpy(&ui, &f, sizeof(float));
  return ui;
}

inline float BitsToFloat(uint32_t ui) {
  float f;
  memcpy(&f, &ui, sizeof(uint32_t));
  return f;
}

inline float NextFloatUp(float v) {
  if (std::isinf(v) && v > 0)
    return v;
  if (v == -0.f)
    v = 0.f;
  uint32_t ui = FloatToBits(v);
  if (v >= 0)
    ++ui;
  else
    --ui;
  return BitsToFloat(ui);
}

inline float NextFloatDown(float v) {
  if (std::isinf(v) && v < 0)
    return v;
  if (v == 0.f)
    v = -0.f;
  uint32_t ui = FloatToBits(v);
  if (v > 0)
    --ui;
  else
    ++ui;
  return BitsToFloat(ui);
}

inline Float gamma(int n) {
  return (n * MachineEpsilon) / (1 - n*MachineEpsilon);
}

} // namespace pbrt

#endif//PBRT_H
