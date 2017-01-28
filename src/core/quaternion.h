#ifndef CORE_QUATERNION_H
#define CORE_QUATERNION_H

#include "pbrt.h"
#include "geometry.h"

#include <algorithm>

namespace pbrt {

struct Quaternion {
public:
  Quaternion() : v(0,0,0), w(1) {}
  Quaternion(const Transform& t);

  Quaternion& operator+=(const Quaternion& q) {
    v += q.v;
    w += q.w;
    return *this;
  }
  friend Quaternion operator+(const Quaternion& q1, const Quaternion& q2) {
    Quaternion ret = q1;
    return ret += q2;
  }
  Quaternion& operator-=(const Quaternion& q) {
    v -= q.v;
    w -= q.w;
    return *this;
  }
  friend Quaternion operator-(const Quaternion& q1, const Quaternion& q2) {
    Quaternion ret = q1;
    return ret -= q2;
  }
  Quaternion operator-() const {
    Quaternion ret;
    ret.v = -v;
    ret.w = -w;
    return ret;
  }
  Quaternion& operator*=(Float f) {
    v *= f;
    w *= f;
    return *this;
  }
  Quaternion operator*(Float f) const {
    Quaternion ret = *this;
    ret.v *= f;
    ret.w *= f;
    return ret;
  }
  Quaternion& operator/=(Float f) {
    v /= f;
    w /= f;
    return *this;
  }
  Quaternion operator/(Float f) const {
    Quaternion ret = *this;
    ret.v /= f;
    ret.w /= f;
    return ret;
  }

  Transform ToTransform() const;

  Vector3f v;
  Float w;
};

inline Quaternion operator*(Float f, const Quaternion& q) {
  return q*f;
}

inline Float Dot(const Quaternion& q1, const Quaternion& q2) {
  return Dot(q1.v, q2.v) + q1.w*q2.w;
}

inline Quaternion Normalize(const Quaternion& q) {
  return q/std::sqrt(Dot(q,q));
}

Quaternion Slerp(Float t, const Quaternion& q1, const Quaternion& q2);

} // namespace pbrt

#endif // CORE_QUATERNION_H
