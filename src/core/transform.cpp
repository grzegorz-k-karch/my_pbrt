#include "transform.h"

#include <cstring>
#include <cmath>

namespace pbrt {
 Matrix4x4::Matrix4x4(Matrix4x4 _m) {
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
     }
 }

} /* namespace pbrt */
