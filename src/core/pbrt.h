#ifndef PBRT_H
#define PBRT_H

#include <assert.h>

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

#endif//PBRT_H
