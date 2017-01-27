#include "transform.h"
#include "error.h"
#include "interaction.h"

#include <cstring>
#include <cmath>
#include <algorithm>

namespace pbrt {

Matrix4x4::Matrix4x4(Float _m[4][4]) {
  std::memcpy(m, _m, 16*sizeof(Float));
}

Matrix4x4::Matrix4x4(Float t00, Float t01, Float t02, Float t03,
    Float t10, Float t11, Float t12, Float t13,
    Float t20, Float t21, Float t22, Float t23,
    Float t30, Float t31, Float t32, Float t33) {

  m[0][0] = t00;
  m[0][1] = t01;
  m[0][2] = t02;
  m[0][3] = t03;
  m[1][0] = t10;
  m[1][1] = t11;
  m[1][2] = t12;
  m[1][3] = t13;
  m[2][0] = t20;
  m[2][1] = t21;
  m[2][2] = t22;
  m[2][3] = t23;
  m[3][0] = t30;
  m[3][1] = t31;
  m[3][2] = t32;
  m[3][3] = t33;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
  return Matrix4x4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
      m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
      m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
      m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
}

Matrix4x4 Inverse(const Matrix4x4& m) {

  int indxc[4], indxr[4];
  int ipiv[4] = {0,0,0,0};
  Float minv[4][4];
  std::memcpy(minv, m.m, 16*sizeof(Float));

  for (int i = 0; i < 4; ++i) {
    int irow = 0, icol = 0;
    Float big = 0.f;
    for (int j = 0; j < 4; ++j) {
      if (ipiv[j] != 1) {
        for (int k = 0; k < 4; ++k) {
          if (ipiv[k] == 0) {
            if (std::abs(minv[j][k]) >= big) {
              big = Float(std::abs(minv[j][k]));
              irow = j;
              icol = k;
            }
          }
          else if (ipiv[k] > 1) {
            Error("Singular matrix in MatrixInvert");
          }
        }
      }
    }
    ++ipiv[icol];
    // swap rows irow and icol for pivot
    if (irow != icol) {
      for (int k = 0; k < 4; ++k) {
        std::swap(minv[irow][k], minv[icol][k]);
      }
    }
    indxr[i] = irow;
    indxc[i] = icol;
    if (minv[icol][icol] == 0.f) {
      Error("Singular matrix in MatrixInvert");
    }

    // set m[icol][icol] to 1 by scaling row icol appropriately
    Float pivinv = 1./minv[icol][icol];
    minv[icol][icol] = 1.;
    for (int j = 0; j < 4; ++j) {
      minv[icol][j] *= pivinv;
    }

    // subtract this row from others to zero-out their columns
    for (int j = 0; j < 4; ++j) {
      if (j != icol) {

        Float save = minv[j][icol];
        minv[j][icol] = 0;

        for (int k = 0; k < 4; ++k) {
          minv[j][k] -= minv[icol][k] * save;
        }
      }
    }
  }
  // swap columns to reflect permutation
  for (int j = 3; j >= 0; --j) {
    if (indxr[j] != indxc[j]) {
      for (int k = 0; k < 4; ++k) {
        std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
      }
    }
  }
  return Matrix4x4(minv);
}

Transform Translate(const Vector3f& delta) {
  Matrix4x4 m(1, 0, 0, delta.x,
      0, 1, 0, delta.y,
      0, 0, 1, delta.z,
      0, 0, 0, 1);
  Matrix4x4 mInv(1, 0, 0, -delta.x,
      0, 1, 0, -delta.y,
      0, 0, 1, -delta.z,
      0, 0, 0, 1);
  return Transform(m, mInv);
}

Transform Scale(Float x, Float y, Float z) {
  Matrix4x4 m(x, 0, 0, 0,
      0, y, 0, 0,
      0, 0, z, 0,
      0, 0, 0, 1);
  Matrix4x4 mInv(1/x, 0, 0, 0,
      0, 1/y, 0, 0,
      0, 0, 1/z, 0,
      0, 0, 0, 1);
  return Transform(m, mInv);
}

Transform RotateX(Float theta) {
  Float sinTheta = std::sin(Radians(theta));
  Float cosTheta = std::cos(Radians(theta));
  Matrix4x4 m(1, 0, 0, 0,
      0, cosTheta, -sinTheta, 0,
      0, sinTheta, cosTheta, 0,
      0, 0, 0, 1);
  return Transform(m, Transpose(m));
}
Transform RotateY(Float theta) {
  Float sinTheta = std::sin(Radians(theta));
  Float cosTheta = std::cos(Radians(theta));
  Matrix4x4 m(cosTheta, 0, sinTheta, 0,
      0, 1, 0, 0,
      -sinTheta, 0, cosTheta, 0,
      0, 0, 0, 1);
  return Transform(m, Transpose(m));
}
Transform RotateZ(Float theta) {
  Float sinTheta = std::sin(Radians(theta));
  Float cosTheta = std::cos(Radians(theta));
  Matrix4x4 m(cosTheta, -sinTheta, 0, 0,
      sinTheta, cosTheta, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1);
  return Transform(m, Transpose(m));
}
Transform Rotate(Float theta, const Vector3f& axis) {
  Vector3f a = Normalize(axis);
  Float sinTheta = std::sin(Radians(theta));
  Float cosTheta = std::cos(Radians(theta));
  Matrix4x4 m;
  m.m[0][0] = a.x*a.x + (1 - a.x*a.x)*cosTheta;
  m.m[0][1] = a.x*a.y*(1 - cosTheta) - a.z*sinTheta;
  m.m[0][2] = a.x*a.z*(1 - cosTheta) + a.y*sinTheta;
  m.m[0][3] = 0;

  m.m[1][0] = a.x*a.y*(1 - cosTheta) + a.z*sinTheta;
  m.m[1][1] = a.y*a.y + (1 - a.y*a.y)*cosTheta;
  m.m[1][2] = a.y*a.z*(1 - cosTheta) - a.x*sinTheta;
  m.m[1][3] = 0;

  m.m[2][0] = a.x*a.z*(1 - cosTheta) - a.y*sinTheta;
  m.m[2][1] = a.y*a.z*(1 - cosTheta) + a.x*sinTheta;
  m.m[2][2] = a.z*a.z + (1 - a.z*a.z)*cosTheta;
  m.m[2][3] = 0;

  return Transform(m, Transpose(m));
}
Transform LookAt(const Point3f& pos, const Point3f& look, const Vector3f& up) {
  Matrix4x4 cameraToWorld;
  cameraToWorld.m[0][3] = pos.x;
  cameraToWorld.m[1][3] = pos.y;
  cameraToWorld.m[2][3] = pos.z;
  cameraToWorld.m[3][3] = 1;

  Vector3f dir = Normalize(look - pos);
  Vector3f left = Normalize(Cross(Normalize(up), dir));
  Vector3f newUp = Cross(dir, left);
  cameraToWorld.m[0][0] = left.x;
  cameraToWorld.m[1][0] = left.y;
  cameraToWorld.m[2][0] = left.z;
  cameraToWorld.m[3][0] = 0;
  cameraToWorld.m[0][1] = newUp.x;
  cameraToWorld.m[1][1] = newUp.y;
  cameraToWorld.m[2][1] = newUp.z;
  cameraToWorld.m[3][1] = 0;
  cameraToWorld.m[0][2] = dir.x;
  cameraToWorld.m[1][2] = dir.y;
  cameraToWorld.m[2][2] = dir.z;
  cameraToWorld.m[3][2] = 0;

  return Transform(Inverse(cameraToWorld), cameraToWorld);
}

Bounds3f Transform::operator()(const Bounds3f& b) const {
  const Transform &M = *this;
  Point3f p0 = M(Point3f(b.pMin.x, b.pMin.y, b.pMin.z));
  Vector3f vx = M(Point3f(b.pMax.x, b.pMin.y, b.pMin.z)-p0);
  Vector3f vy = M(Point3f(b.pMin.x, b.pMax.y, b.pMin.z)-p0);
  Vector3f vz = M(Point3f(b.pMin.x, b.pMin.y, b.pMax.z)-p0);

  Bounds3f ret(p0);
  ret = Union(ret, p0+vx);
  ret = Union(ret, p0+vy);
  ret = Union(ret, p0+vz);
  ret = Union(ret, p0+vy+vz);
  ret = Union(ret, p0+vx+vy);
  ret = Union(ret, p0+vx+vz);
  ret = Union(ret, p0+vx+vy+vz);
  return ret;
}

Transform Transform::operator*(const Transform& t2) const {
  return Transform(Matrix4x4::Mul(m, t2.m), Matrix4x4::Mul(t2.mInv, mInv));
}

SurfaceInteraction Transform::operator()(const SurfaceInteraction& si) const {
  SurfaceInteraction ret;

  // <transform p and pError in SurfaceInteraction>
  ret.p = (*this)(si.p, si.pError, &ret.pError);

  // <transform remaining members of SurfaceInteraction>
  const Transform &t = *this;
  ret.n = Normalize(t(si.n));
  ret.wo = Normalize(t(si.wo));
  ret.time = si.time;
  // ret.mediumInterface = si.mediumInterface;
  ret.uv = si.uv;
  ret.shape = si.shape;
  ret.dpdu = t(si.dpdu);
  ret.dpdv = t(si.dpdv);
  ret.dndu = t(si.dndu);
  ret.dndv = t(si.dndv);
  ret.shading.n = Normalize(t(si.shading.n));
  ret.shading.dpdu = t(si.shading.dpdu);
  ret.shading.dpdv = t(si.shading.dpdv);
  ret.shading.dndu = t(si.shading.dndu);
  ret.shading.dndv = t(si.shading.dndv);
//  ret.dudx = si.dudx; // TODO
//  ret.dvdx = si.dvdx; // TODO
//  ret.dudy = si.dudy; // TODO
//  ret.dvdy = si.dvdy; // TODO
//  ret.dpdx = t(si.dpdx); // TODO
//  ret.dpdy = t(si.dpdy); // TODO
//  ret.bsdf = si.bsdf; // TODO
//  ret.bssrdf = si.bssrdf; // TODO
//  ret.primitive = si.primitive; // TODO
  //    ret.n = Faceforward(ret.n, ret.shading.n); // was commented...
  ret.shading.n = Faceforward(ret.shading.n, ret.n);

  return ret;
}

bool Transform::SwapsHandedness() const {
  Float det =
      m.m[0][0]*(m.m[1][1]*m.m[2][2] - m.m[1][2]*m.m[2][1]) -
      m.m[0][1]*(m.m[1][0]*m.m[2][2] - m.m[1][2]*m.m[2][0]) +
      m.m[0][2]*(m.m[1][0]*m.m[2][1] - m.m[1][1]*m.m[2][0]);
  return det < 0;
}

AnimatedTransform::AnimatedTransform(const Transform* startTransform, Float startTime,
    const Transform* endTransform, Float endTime)
: startTransform(startTransform), endTransform(endTransform),
  startTime(startTime), endTime(endTime),
  actuallyAnimated(*startTransform != *endTransform) {

  // TODO
}

} /* namespace pbrt */
