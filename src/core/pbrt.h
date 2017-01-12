#ifndef PBRT_H
#define PBRT_H

#include <assert.h>
#include <limits>

#ifdef PBRT_FLOAT_AS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif//PBRT_FLOAT_AS_DOUBLE

typedef Float RGBSpectrum; // <gkk>

typedef RGBSpectrum Spectrum;

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
static constexpr Float Infinity = std::numeric_limits<Float>::infinity();

static constexpr Float Pi = 3.14159265358979323846;

inline Float Radians(Float deg) { return (Pi/180)*deg; }
inline Float Degrees(Float rad) { return (180/Pi)*rad; }

#endif//PBRT_H
