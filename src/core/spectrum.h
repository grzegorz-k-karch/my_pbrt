#ifndef CORE_SPECTRUM_H
#define CORE_SPECTRUM_H

#include "pbrt.h"

namespace pbrt {

static const int nSpectralSamples = 60;
enum class SpectrumType {Reflectance, Illuminant};


template<int nSpectrumSamples>
class CoefficientSpectrum {
public:
  CoefficientSpectrum(Float v = 0.0f) {
    for (int i = 0; i < nSpectrumSamples; ++i) {
      c[i] = v;
    }
  }

  CoefficientSpectrum& operator+=(const CoefficientSpectrum& s) {
    for (int i = 0; i < nSpectrumSamples; ++i) {
      c[i] += s.c[i];
    }
    return *this;
  }
  CoefficientSpectrum operator+(const CoefficientSpectrum& s) const {
    CoefficientSpectrum ret = *this;
    for (int i = 0; i < nSpectrumSamples; ++i) {
      ret.c[i] += s.c[i];
    }
    return ret;
  }
  CoefficientSpectrum& operator-=(const CoefficientSpectrum& s) {
    for (int i = 0; i < nSpectrumSamples; ++i) {
      c[i] -= s.c[i];
    }
    return *this;
  }
  CoefficientSpectrum operator-(const CoefficientSpectrum& s) const {
    CoefficientSpectrum ret = *this;
    for (int i = 0; i < nSpectrumSamples; ++i) {
      ret.c[i] -= s.c[i];
    }
    return ret;
  }
  CoefficientSpectrum& operator*=(const CoefficientSpectrum& s) {
    for (int i = 0; i < nSpectrumSamples; ++i) {
      c[i] *= s.c[i];
    }
    return *this;
  }
  CoefficientSpectrum operator*(const CoefficientSpectrum& s) const {
    CoefficientSpectrum ret = *this;
    for (int i = 0; i < nSpectrumSamples; ++i) {
      ret.c[i] *= s.c[i];
    }
    return ret;
  }
  CoefficientSpectrum& operator*=(Float s) {
    for (int i = 0; i < nSpectrumSamples; ++i) {
      c[i] *= s;
    }
    return *this;
  }
  CoefficientSpectrum operator*(Float s) const {
    CoefficientSpectrum ret = *this;
    for (int i = 0; i < nSpectrumSamples; ++i) {
      ret.c[i] *= s;
    }
    return ret;
  }

  CoefficientSpectrum operator-() const {
    CoefficientSpectrum ret = *this;
    for (int i = 0; i < nSpectrumSamples; ++i) {
      ret.c[i] *= -1.f;
    }
    return ret;
  }
  bool operator==(const CoefficientSpectrum& s) {
    for (int i = 0; i < nSpectrumSamples; ++i) {
      if (c[i] != s.c[i]) {
        return false;
      }
    }
    return true;
  }
  bool operator!=(const CoefficientSpectrum& s) {
    return !(*this == s);
  }
  bool IsBlack() const {
    for (int i = 0; i < nSpectrumSamples; ++i) {
      if (c[i] != 0.0f) {
        return false;
      }
    }
    return true;
  }

  friend CoefficientSpectrum Sqrt(const CoefficientSpectrum& s) {
    CoefficientSpectrum ret;
    for (int i = 0; i < nSpectrumSamples; ++i) {
      ret.c[i] = std::sqrt(s.c[i]);
    }
    return ret;
  }

  friend inline CoefficientSpectrum operator*(Float a, const CoefficientSpectrum& s) {
    return s*a;
  }

  CoefficientSpectrum Clamp(Float low = 0, Float high = Infinity) const {
    CoefficientSpectrum ret;
    for (int i = 0; i < nSpectrumSamples; ++i) {
      ret.c[i] = pbrt::Clamp(c[i], low, high);
    }
    return ret;
  }

  CoefficientSpectrum HasNaNs() const {
    for (int i = 0; i < nSpectrumSamples; ++i) {
      if (std::isnan(c[i])) {
        return true;
      }
    }
    return false;
  }

  Float& operator[](int i) {
    return c[i];
  }

  static const int nSamples = nSpectrumSamples;

protected:
  Float c[nSpectrumSamples];
};

class SampledSpectrum : public CoefficientSpectrum<nSpectralSamples> {
};

class RGBSpectrum : public CoefficientSpectrum<3> {
public:
  RGBSpectrum(Float v = 0.0f) : CoefficientSpectrum<3>(v) {}
  RGBSpectrum(const CoefficientSpectrum<3>& v) : CoefficientSpectrum<3>(v) {}

  static RGBSpectrum FromRGB(const Float rgb[3], SpectrumType = SpectrumType::Reflectance) {
    RGBSpectrum s;
    s.c[0] = rgb[0];
    s.c[1] = rgb[1];
    s.c[2] = rgb[2];
    return s;
  }
  void ToRGB(Float* rgb) const {
    rgb[0] = c[0];
    rgb[1] = c[1];
    rgb[2] = c[2];
  }

  const RGBSpectrum& ToRGBSpectrum() const {
    return *this;
  }

  void ToXYZ(Float xyz[3]) const {
    xyz[0] = 0.412453f*c[0] + 0.357580f*c[1] + 0.180423f*c[2];
    xyz[1] = 0.212671f*c[0] + 0.715160f*c[1] + 0.072169f*c[2];
    xyz[2] = 0.019334f*c[0] + 0.119193f*c[1] + 0.950227f*c[2];
  }

  static RGBSpectrum FromXYZ(const Float xyz[3]) {
    RGBSpectrum s;
    s.c[0] =  3.240479f*xyz[0] - 1.537150f*xyz[1] - 0.498535f*xyz[2];
    s.c[1] = -0.969256f*xyz[0] + 1.875991f*xyz[1] + 0.041556f*xyz[2];
    s.c[2] =  0.055648f*xyz[0] - 0.204043f*xyz[1] + 1.057311f*xyz[2];

    return s;
  }

  static RGBSpectrum FromSampled(const Float* lambda, const Float* v, int n) {
    // TODO
  }
};

inline Spectrum Lerp(Float t, const Spectrum& s1, const Spectrum& s2) {
  return (1 - t)*s1 + t*s2;
}


} // namespace pbrt

#endif // CORE_SPECTRUM_H
