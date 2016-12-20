#ifndef PBRT_H
#define PBRT_H

#ifdef PBRT_FLOAT_AS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif//PBRT_FLOAT_AS_DOUBLE

typedef Float RGBSpectrum; // <gkk>

typedef RGBSpectrum Spectrum;

#define ALLOCA(TYPE, COUNT) (TYPE*)alloca((COUNT)*sizeof(TYPE))

#endif//PBRT_H
