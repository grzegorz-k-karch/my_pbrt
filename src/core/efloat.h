#ifndef CORE_EFLOAT_H
#define CORE_EFLOAT_H

#include "pbrt.h"
#include <algorithm>

namespace pbrt {

class EFloat {

public:
  EFloat() {}
  EFloat(float _v, float err = 0.f)
  : v(_v) {
    if (err == 0) {
      low = high = v;
    }
    else {
      low = NextFloatDown(v - err);
      high = NextFloatUp(v + err);
    }
    // TODO <store high precision reference value in Float>
  }

  explicit operator float() const {
    return v;
  }
  explicit operator double() const {
    return v;
  }

  inline bool operator==(EFloat f) const {
    return v == f.v;
  }

  EFloat operator+(EFloat f) const {
    EFloat r;
    r.v = v + f.v;
    r.low = NextFloatDown(LowerBound() + f.LowerBound());
    r.high = NextFloatUp(UpperBound() + f.UpperBound());
    return r;
  }
  EFloat operator-(EFloat f) const {
    EFloat r;
    r.v = v - f.v;
    r.low = NextFloatDown(LowerBound() - f.UpperBound());
    r.high = NextFloatUp(UpperBound() - f.LowerBound());
    return r;
  }
  EFloat operator*(EFloat f) const {
    EFloat r;
    r.v = v*f.v;
    Float prod[4] = {LowerBound()*f.LowerBound(),
                     UpperBound()*f.LowerBound(),
                     LowerBound()*f.UpperBound(),
                     UpperBound()*f.UpperBound()};

    r.low = NextFloatDown(std::min(std::min(prod[0], prod[1]), std::min(prod[2], prod[3])));
    r.high = NextFloatUp(std::max(std::max(prod[0], prod[1]), std::max(prod[2], prod[3])));
    return r;
  }
  EFloat operator/(EFloat f) const {
    EFloat r;
    r.v = v/f.v;
    if (f.low < 0 && f.high > 0) {
      r.low = -Infinity;
      r.high = Infinity;
    }
    else {
      Float div[4] = {LowerBound()/f.LowerBound(),
                      UpperBound()/f.LowerBound(),
                      LowerBound()/f.UpperBound(),
                      UpperBound()/f.UpperBound()};

      r.low = NextFloatDown(std::min(std::min(div[0], div[1]), std::min(div[2], div[3])));
      r.high = NextFloatUp(std::max(std::max(div[0], div[1]), std::max(div[2], div[3])));
    }
    return r;
  }

  float GetAbsoluteError() const {
    return high - low;
  }
  float UpperBound() const {
    return high;
  }
  float LowerBound() const {
    return low;
  }

private:
  float v;
  float low, high;

  friend inline EFloat sqrt(EFloat f);
};

inline EFloat operator+(float f, EFloat fe) {
  return EFloat(f) + fe;
}

inline EFloat operator-(float f, EFloat fe) {
  return EFloat(f) - fe;
}

inline EFloat operator*(float f, EFloat fe) {
  return EFloat(f)*fe;
}

inline EFloat operator/(float f, EFloat fe) {
  return EFloat(f)/fe;
}

inline EFloat sqrt(EFloat f) {
  EFloat r;
  r.v = std::sqrt(f.v);
  r.low = NextFloatDown(std::sqrt(f.low));
  r.high = NextFloatUp(std::sqrt(f.high));
  return r;
}

inline bool Quadratic(EFloat a, EFloat b, EFloat c, EFloat* t0, EFloat* t1) {
  double discrim = (double)b*(double)b - 4*(double)a*(double)c;
  if (discrim < 0)
    return false;
  double rootDiscrim = std::sqrt(discrim);
  EFloat floatRootDiscrim(rootDiscrim, MachineEpsilon*rootDiscrim);
  EFloat q;
  if ((float)b < 0)
    q = -0.5*(b - floatRootDiscrim);
  else
    q = -0.5*(b + floatRootDiscrim);
  *t0 = q/a;
  *t1 = c/q;
  if ((float)*t0 > (float)*t1)
    std::swap(*t0, *t1);
  return true;
}

} // namespace pbrt

#endif // CORE_EFLOAT_H
