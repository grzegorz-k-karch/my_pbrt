#ifndef CORE_TRANSFORM_H
#define CORE_TRANSFORM_H

#include "pbrt.h"
#include "geometry.h"
#include "quaternion.h"

namespace pbrt {

struct Matrix4x4 {
  Matrix4x4() {
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
    m[0][1] = m[0][2] = m[0][3] =
        m[1][0] = m[1][2] = m[1][2] =
            m[2][0] = m[2][1] = m[2][3] =
                m[3][0] = m[3][1] = m[3][2] = 0.f;
  }
  Matrix4x4(Float _m[4][4]);
  Matrix4x4(Float t00, Float t01, Float t02, Float t03,
      Float t10, Float t11, Float t12, Float t13,
      Float t20, Float t21, Float t22, Float t23,
      Float t30, Float t31, Float t32, Float t33);

  friend Matrix4x4 Transpose(const Matrix4x4&);
  friend Matrix4x4 Inverse(const Matrix4x4&);

  static Matrix4x4 Mul(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 r;
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        r.m[i][j] += m1.m[i][0]*m2.m[0][j] +
        m1.m[i][1]*m2.m[1][j] +
        m1.m[i][2]*m2.m[2][j] +
        m1.m[i][3]*m2.m[3][j];
    return r;
  }

  bool operator==(const Matrix4x4& m2) const {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        if (m[i][j] != m2.m[i][j]) return false;
    return true;
  }
  bool operator!=(const Matrix4x4& m2) const {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j)
        if (m[i][j] != m2.m[i][j]) return true;
    return false;
  }

  Float m[4][4];
};

class Transform {
public:
  Transform() {}
  Transform(const Float mat[4][4]) {
    m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
        mat[1][0], mat[1][1], mat[1][2], mat[1][3],
        mat[2][0], mat[2][1], mat[2][2], mat[2][3],
        mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
    mInv = Inverse(m);
  }
  Transform(const Matrix4x4& _m) :
    m(_m), mInv(Inverse(_m)) { }

  Transform(const Matrix4x4& _m, const Matrix4x4& _mInv) :
    m(_m), mInv(_mInv) { }

  friend Transform Inverse(const Transform& t) {
    return Transform(t.mInv, t.m);
  }
  friend Transform Transpose(const Transform& t) {
    return Transform(Transpose(t.m), Transpose(t.mInv));
  }

  bool operator==(const Transform& t) const {
    return m == t.m && mInv == t.mInv;
  }
  bool operator!=(const Transform& t) const {
    return m != t.m || mInv != t.mInv;
  }
  Transform operator*(const Transform& t2) const;

  bool IsIdentity() const {
    return (m.m[0][0] == 1.f && m.m[0][0] == 0.f && m.m[0][0] == 0.f && m.m[0][0] == 0.f &&
        m.m[0][0] == 0.f && m.m[0][0] == 1.f && m.m[0][0] == 0.f && m.m[0][0] == 0.f &&
        m.m[0][0] == 0.f && m.m[0][0] == 0.f && m.m[0][0] == 1.f && m.m[0][0] == 0.f &&
        m.m[0][0] == 0.f && m.m[0][0] == 0.f && m.m[0][0] == 0.f && m.m[0][0] == 1.f);
  }

  bool HasScale() const {
    Float la2 = (*this)(Vector3f(1, 0, 0)).LengthSquared();
    Float lb2 = (*this)(Vector3f(0, 1, 0)).LengthSquared();
    Float lc2 = (*this)(Vector3f(0, 0, 1)).LengthSquared();
#define NOT_ONE(x) ((x) < .999f || (x) > 1.001f)
            return (NOT_ONE(la2) || NOT_ONE(lb2) || NOT_ONE(lc2));
#undef NOT_ONE
  }

  bool SwapsHandedness() const;

  template <typename T>
  inline Point3<T> operator ()(const Point3<T>& p) const;
  template <typename T>
  inline Point3<T> operator ()(const Point3<T>& p, Vector3<T>* pError) const;
  template <typename T>
  inline Point3<T> operator ()(const Point3<T>& p, const Vector3<T>& pError, Vector3<T>* pTransError) const;
  template <typename T>
  inline Vector3<T> operator()(const Vector3<T>& v) const;
  template <typename T>
  inline Vector3<T> operator()(const Vector3<T>& v, Vector3<T>* vTransError) const;
  template <typename T>
  inline Vector3<T> operator()(const Vector3<T>& v, const Vector3<T>& vError, Vector3<T>* vTransError) const;
  template <typename T>
  inline Normal3<T> operator ()(const Normal3<T>& n) const;

  template <typename T> inline Bounds3<T> operator ()(const Bounds3<T>& n) const;
  inline Ray operator()(const Ray& r) const;
  inline Ray operator()(const Ray& r, Vector3f* oError, Vector3f* dError) const;
  Bounds3f operator()(const Bounds3f& b) const;
  SurfaceInteraction operator()(const SurfaceInteraction& si) const;

private:
  Matrix4x4 m, mInv;
  friend class AnimatedTransform;
  friend struct Quaternion;
};

template <typename T> inline Point3<T>
Transform::operator ()(const Point3<T>& p) const {
  T x = p.x, y = p.y, z = p.z;
  T xp = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
  T yp = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
  T zp = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
  T wp = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];
  if (wp == 1) return Point3<T>(xp, yp, zp);
  else return Point3<T>(xp, yp, zp)/wp;
}

template <typename T>
inline Point3<T> Transform::operator()(const Point3<T>& p, Vector3<T>* pError) const {
  T x = p.x, y = p.y, z = p.z;
  T xp = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
  T yp = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
  T zp = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
  T wp = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];

  T xAbsSum = (std::abs(m.m[0][0]*x) + std::abs(m.m[0][1]*y)+
      std::abs(m.m[0][2]*z) + std::abs(m.m[0][3]));
  T yAbsSum = (std::abs(m.m[1][0]*x) + std::abs(m.m[1][1]*y)+
      std::abs(m.m[1][2]*z) + std::abs(m.m[1][3]));
  T zAbsSum = (std::abs(m.m[2][0]*x) + std::abs(m.m[2][1]*y)+
      std::abs(m.m[2][2]*z) + std::abs(m.m[2][3]));

  *pError = gamma(3) * Vector3<T>(xAbsSum, yAbsSum, zAbsSum);
  // TODO CHECK_NE(wp, 0);
  if (wp == 1)
    return Point3<T>(xp, yp, zp);
  else
    return Point3<T>(xp, yp, zp)/wp;
}

template <typename T>
inline Point3<T> Transform::operator ()(const Point3<T>& p, const Vector3<T>& pError,
    Vector3<T>* pTransError) const {
  T x = p.x, y = p.y, z = p.z;
  T xp = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
  T yp = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
  T zp = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
  T wp = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];

  pTransError->x =
      (gamma(3) + (T)1)*
      (std::abs(m.m[0][0])*pError.x + std::abs(m.m[0][1])*pError.y + std::abs(m.m[0][2])*pError.z) +
      gamma(3)*
      (std::abs(m.m[0][0]*x) + std::abs(m.m[0][1]*y) + std::abs(m.m[0][2]*z) + std::abs(m.m[0][3]));
  pTransError->x =
      (gamma(3) + (T)1)*
      (std::abs(m.m[1][0])*pError.x + std::abs(m.m[1][1])*pError.y + std::abs(m.m[1][2])*pError.z) +
      gamma(3)*
      (std::abs(m.m[1][0]*x) + std::abs(m.m[1][1]*y) + std::abs(m.m[1][2]*z) + std::abs(m.m[1][3]));
  pTransError->x =
      (gamma(3) + (T)1)*
      (std::abs(m.m[2][0])*pError.x + std::abs(m.m[2][1])*pError.y + std::abs(m.m[2][2])*pError.z) +
      gamma(3)*
      (std::abs(m.m[2][0]*x) + std::abs(m.m[2][1]*y) + std::abs(m.m[2][2]*z) + std::abs(m.m[2][3]));

  // TODO CHECK_NE(wp, 0);
  if (wp == 1) {
    return Point3<T>(xp, yp, zp);
  }
  else {
    return Point3<T>(xp, yp, zp)/wp;
  }
}


template <typename T> inline Vector3<T>
Transform::operator()(const Vector3<T>& v) const {
  T x = v.x, y = v.y, z = v.z;
  return Vector3<T>(m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
      m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
      m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z);
}

template <typename T>
inline Vector3<T> Transform::operator()(const Vector3<T>& v, Vector3<T>* vError) const {

  T x = v.x, y = v.y, z = v.z;
  vError->x = gamma(3)*(std::abs(m.m[0][0]*x) +
      std::abs(m.m[0][1]*y) + std::abs(m.m[0][2]*z));
  vError->y = gamma(3)*(std::abs(m.m[1][0]*x) +
      std::abs(m.m[1][1]*y) + std::abs(m.m[1][2]*z));
  vError->z = gamma(3)*(std::abs(m.m[2][0]*x) +
      std::abs(m.m[2][1]*y) + std::abs(m.m[2][2]*z));

  return Vector3<T>(m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
      m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
      m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z);
}

template <typename T>
inline Vector3<T> Transform::operator()(const Vector3<T>& v, const Vector3<T>& vError,
    Vector3<T>* vTransError) const {

  T x = v.x, y = v.y, z = v.z;

  vTransError->x =
      (gamma(3) + (T)1)*
      (std::abs(m.m[0][0])*vError.x + std::abs(m.m[0][1])*vError.y + std::abs(m.m[0][2])*vError.z) +
      gamma(3)*
      (std::abs(m.m[0][0]*x) + std::abs(m.m[0][1]*y) + std::abs(m.m[0][2]*z));
  vTransError->x =
      (gamma(3) + (T)1)*
      (std::abs(m.m[1][0])*vError.x + std::abs(m.m[1][1])*vError.y + std::abs(m.m[1][2])*vError.z) +
      gamma(3)*
      (std::abs(m.m[1][0]*x) + std::abs(m.m[1][1]*y) + std::abs(m.m[1][2]*z));
  vTransError->x =
      (gamma(3) + (T)1)*
      (std::abs(m.m[2][0])*vError.x + std::abs(m.m[2][1])*vError.y + std::abs(m.m[2][2])*vError.z) +
      gamma(3)*
      (std::abs(m.m[2][0]*x) + std::abs(m.m[2][1]*y) + std::abs(m.m[2][2]*z));

  return Vector3<T>(m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
      m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
      m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z);
}


template <typename T> inline Normal3<T>
Transform::operator()(const Normal3<T>& n) const {
  T x = n.x, y = n.y, z = n.z;
  return Normal3<T>(mInv.m[0][0]*x + mInv.m[1][0]*y + mInv.m[2][0]*z,
      mInv.m[0][1]*x + mInv.m[1][1]*y + mInv.m[2][1]*z,
      mInv.m[0][2]*x + mInv.m[1][2]*y + mInv.m[2][2]*z);
}

template <typename T> inline Bounds3<T>
Transform::operator()(const Bounds3<T>& b) const {
  T x = b.x, y = b.y, z = b.z;
  return Bounds3<T>(m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
      m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
      m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z);
}

inline Ray Transform::operator()(const Ray& r) const {
  Vector3f oError;
  Point3f o = (*this)(r.o, &oError);
  Vector3f d = (*this)(r.d);

  // <offset ray origin to edge of error bounds and compute tMax>
  Float lengthSquared = d.LengthSquared();
  Float tMax = r.tMax;
  if (lengthSquared > 0) {
    Float dt = Dot(Abs(d), oError) / lengthSquared;
    o += d*dt;
    tMax -= dt;
  }

  return Ray(o, d, tMax, r.time, r.medium);
}

inline Ray Transform::operator()(const Ray& r, Vector3f* oError, Vector3f* dError) const {

  Point3f o = (*this)(r.o, oError);
  Vector3f d = (*this)(r.d, dError);
  Float tMax = r.tMax;
  Float lengthSquared = d.LengthSquared();
  if (lengthSquared > 0) {
    Float dt = Dot(Abs(d), *oError) / lengthSquared;
    o += d*dt;
    // tMax -= dt; // was commented in pbrt...
  }
  return Ray(o, d, tMax, r.time, r.medium);
}

Transform Translate(const Vector3f& delta);
Transform Scale(Float x, Float y, Float z);
Transform RotateX(Float theta);
Transform RotateY(Float theta);
Transform RotateZ(Float theta);
Transform Rotate(Float theta, const Vector3f& axis);
Transform LookAt(const Point3f& pos, const Point3f& look, const Vector3f& up);
Transform Orthographic(Float zNear, Float zFar);
Transform Perspective(Float fov, Float zNear, Float zFar);

class AnimatedTransform {
public:
    AnimatedTransform(const Transform* startTransform, Float startTime,
      const Transform* endTransform, Float endTime);
    void Decompose(const Matrix4x4& m, Vector3f* T, Quaternion* Rquat, Matrix4x4* S);
    void Interpolate(Float time, Transform* t) const;
    Ray operator()(const Ray& r) const;
    RayDifferential operator()(RayDifferential& r) const;
    Point3f operator()(Float time, const Point3f& p) const;
    Vector3f operator()(Float time, const Vector3f& v) const;
    Bounds3f MotionBounds(const Bounds3f& b) const;
private:
  const Transform *startTransform, *endTransform;
  const Float startTime, endTime;
  const bool actuallyAnimated;

  Vector3f T[2];
  Quaternion R[2];
  Matrix4x4 S[2];
  bool hasRotation;
};

} // namespace pbrt

#endif//CORE_TRANSFORM_H
