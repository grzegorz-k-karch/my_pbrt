#ifndef CORE_TRANSFORM_H
#define CORE_TRANSFORM_H

#include "pbrt.h"
#include "geometry.h"

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

    template <typename T>
    inline Vector3<T> operator()(const Vector3<T>& v) const;

private:
    Matrix4x4 m, mInv;
};

Transform Translate(const Vector3f& delta);
Transform Scale(Float x, Float y, Float z);
Transform RotateX(Float theta);
Transform RotateY(Float theta);
Transform RotateZ(Float theta);
Transform Rotate(Float theta, const Vector3f& axis);
Transform LookAt(const Point3f& pos, const Point3f& look, const Vector3f& up);

    template <typename T> inline Vector3<T>
    Transform::operator()(const Vector3<T>& v) const {
        T x = v.x, y = v.y, z = v.z;
        return Vector3<T>(m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
                          m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
                          m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z);
    }
} /* namespace pbrt */

#endif//CORE_TRANSFORM_H
