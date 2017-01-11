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
    Float m[4][4];
};

class Transform {
public:

private:
    Matrix4x4 m, mInv;
};

} /* namespace pbrt */

#endif//CORE_TRANSFORM_H
