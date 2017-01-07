#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "pbrt.h"
#include <limits>
#include <algorithm>
#include <cmath>

namespace pbrt {


  class Ray {

  };

  class RayDifferential : public Ray {

  };

  //--------------------------------------------------------------------------
  template <typename T> class Vector2 {

  public:
    Vector2() { x = y = 0; }

    Vector2(T _x, T _y) : x(_x), y(_y) {
      Assert(!HasNaNs());
    }

    T operator[](int i) const {
      Assert(i == 0 || i == 1);
      if (i == 0) return x;
      return y;
    }
    T& operator[](int i) {
      Assert(i == 0 || i == 1);
      if (i == 0) return x;
      return y;
    }
    Vector2<T> operator+(const Vector2<T>& v) const {
      return Vector2(x + v.x, y + v.y);
    }
    Vector2<T>& operator+=(const Vector2<T>& v) {
      x += v.x; y + v.y;;
      return *this;
    }
    Vector2<T> operator-(const Vector2<T>& v) const {
      return Vector2(x - v.x, y - v.y);
    }
    Vector2<T>& operator-=(const Vector2<T>& v) {
      x -= v.x; y - v.y;;
      return *this;
    }
    Vector2<T> operator*(T s) const {
      return Vector2(s*x, s*y);
    }
    Vector2<T>& operator*=(T s) {
      x *= s; y *= s;
      return *this;
    }
    Vector2<T> operator/(T s) const {
      Assert(s != 0);
      Float inv = 1/s;
      return Vector2(x*inv, y*inv);
    }
    Vector2<T>& operator/=(T s) {
      Assert(s != 0);
      Float inv = 1/s;
      x *= inv; y *= inv;
      return *this;
    }
    Vector2<T> operator-() const { return Vector2<T>(-x, -y); }

    bool HasNaNs() const {
      return std::isnan(x) || std::isnan(y);
    }

    T x, y;
  };

  template <typename T> inline Vector2<T>
    operator*(T s, const Vector2<T>& v) { return v*s; }

  template <typename T> inline Vector2<T>
    Abs(const Vector2<T>& v) { return Vector2<T>(std::abs(v.x), std::abs(v.y)); }

  //--------------------------------------------------------------------------
  template <typename T> class Vector3 {

  public:
    Vector3() { x = y = z = 0; }
  Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {
      Assert(!HasNaNs());
    }

    T operator[](int i) const {
      Assert(i >= 0 && i <= 2);
      if (i == 0) return x;
      if (i == 1) return y;
      return z;
    }
    T& operator[](int i) {
      Assert(i >= 0 && i <= 2);
      if (i == 0) return x;
      if (i == 1) return y;
      return z;
    }
    Vector3<T> operator+(const Vector3<T>& v) const {
      return Vector3(x + v.x, y + v.y, z + v.z);
    }
    Vector3<T>& operator+=(const Vector3<T>& v) {
      x += v.x; y + v.y; z + v.z;
      return *this;
    }
    Vector3<T> operator-(const Vector3<T>& v) const {
      return Vector3(x - v.x, y - v.y, z - v.z);
    }
    Vector3<T>& operator-=(const Vector3<T>& v) {
      x -= v.x; y - v.y; z - v.z;
      return *this;
    }
    Vector3<T> operator*(T s) const {
      return Vector3(s*x, s*y, s*z);
    }
    Vector3<T>& operator*=(T s) {
      x *= s; y *= s; z *= s;
      return *this;
    }
    Vector3<T> operator/(T s) const {
      Assert(s != 0);
      Float inv = 1/s;
      return Vector3(x*inv, y*inv, z*inv);
    }
    Vector3<T>& operator/=(T s) {
      Assert(s != 0);
      Float inv = 1/s;
      x *= inv; y *= inv; z *= inv;
      return *this;
    }
    Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }

    Float LengthSquared() const { return x*x + y*y + z*z; }
    Float Length() const { return std::sqrt(LengthSquared()); }

    bool HasNaNs() const {
      return std::isnan(x) || std::isnan(y) || std::isnan(z);
    }

    T x, y, z;
  };

  template <typename T> inline Vector3<T>
    operator*(T s, const Vector3<T>& v) { return v*s; }

  template <typename T> inline Vector3<T>
    Abs(const Vector3<T>& v) {
      return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
  }

  template <typename T> inline T
  Dot(const Vector3<T>& v1, const Vector3<T>& v2) {
      return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
  }

  template <typename T> inline T
  AbsDot(const Vector3<T>& v1, const Vector3<T>& v2) {
      return std::abs(Dot(v1, v2));
  }

  template <typename T> inline Vector3<T>
  Cross(const Vector3<T>& v1, const Vector3<T>& v2) {
      double v1x = v1.x, v1y = v1.y, v1z = v1.z;
      double v2x = v2.x, v2y = v2.y, v2z = v2.z;
      return Vector3<T>(v1y*v2z - v1z*v2y,
                        v1z*v2x - v1x*v2z,
                        v1x*v2y - v1y*v2x);
  }

  template <typename T> inline Vector3<T>
  Normalize(const Vector3<T>& v) {
      return v / v.Length();
  }

  template <typename T> inline T
  MinComponent(const Vector3<T>& v) {
      return std::min(v.x, std::min(v.y, v.z));
  }

  template <typename T> inline T
  MaxComponent(const Vector3<T>& v) {
      return std::max(v.x, std::max(v.y, v.z));
  }

  template <typename T> int
  MaxDimension(const Vector3<T>& v) {
      return (v.x > v.y) ? (v.x > v.z ? 0 : 2) : (v.y > v.z ? 1 : 2);
  }

  template <typename T> inline Vector3<T>
  MinComponent(const Vector3<T>& v1, const Vector3<T>& v2) {
      return Vector3<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
  }

  template <typename T> inline Vector3<T>
  MaxComponent(const Vector3<T>& v1, const Vector3<T>& v2) {
      return Vector3<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
  }

  template <typename T> inline Vector3<T>
  Permute(const Vector3<T>& v, int x, int y, int z) {
      return Vector3<T>(v[x], v[y], v[z]);
  }

  template <typename T> inline void
  CoordinateSystem(const Vector3<T>& v1, Vector3<T>* v2, Vector3<T>* v3) {
      if (std::abs(v1.x) > std::abs(v1.y)) {
          *v2 = Vector3<T>(-v1.z, 0, v1.x) / std::sqrt(v1.x*v1.x + v1.z*v1.z);
      }
      else {
          *v2 = Vector3<T>(0, v1.z, -v1.y) / std::sqrt(v1.y*v1.y + v1.z*v1.z);
      }
      *v3 = Cross(v1, *v2);
  }

  typedef Vector2<int> Vector2i;
  typedef Vector2<float> Vector2f;
  typedef Vector3<int> Vector3i;
  typedef Vector3<float> Vector3f;

  template <typename T> class Point3;

  // <point declarations>
  template <typename T> class Point2 {

  public:
    Point2() { x = y = 0; }
  Point2(T _x, T _y) : x(_x), y(_y) {}

    template <typename U>
      explicit operator Point2<U>() const {
      return Point2<U>(x, y);
    }

    Vector2<T> operator-(const Point2<T>& p) const {
      return Vector2<T>(x - p.x, y - p.y);
    }

    explicit Point2(const Point3<T>& p) : x(p.x), y(p.y) {
        Assert(!HasNaNs());
    }

    bool HasNaNs() const {
      return std::isnan(x) || std::isnan(y);
    }

    T x, y;
  };

  template <typename T> class Point3 {

  public:
    Point3() { x = y = z = 0; }
    Point3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {
        Assert(!HasNaNs());
    }

    template <typename U> explicit Point3(const Point3<U>& p)
    : x((T)p.x), y((T)p.y), z((T)p.z) {
        Assert(!HasNaNs());
    }

    template <typename U> explicit operator Vector3<U>() const {
        return Vector3<U>(x, y, z);
    }

    Point3<T> operator+(const Vector3<T>& v) const {
        return Point3<T>(x + v.x, y + v.y, z + v.z);
    }

    Point3<T>& operator+=(const Vector3<T>& v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    Vector3<T> operator-(const Point3<T>& p) const {
        return Vector3<T>(x - p.x, y - p.y, z - p.z);
    }

    Point3<T> operator-(const Vector3<T>& v) const {
        return Point3<T>(x - v.x, y - v.y, z - v.z);
    }

    Point3<T>& operator-=(const Vector3<T>& v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    bool HasNaNs() const {
      return std::isnan(x) || std::isnan(y) || std::isnan(z);
    }

    T x, y, z;
  };

  template <typename T> inline Float
  Distance(const Point3<T>& p1, const Point3<T>& p2) {
      return (p1 - p2).Length();
  }

  template <typename T> inline Float
  DistanceSquared(const Point3<T>& p1, const Point3<T>& p2) {
      return (p1 - p2).LengthSquared();
  }

  template <typename T> inline Point3<T>
  Lerp(Float t, const Point3<T>& p0, const Point3<T>& p1) {
      return (1 - t)*p0 + t*p1;
  }

  template <typename T> inline Point3<T>
  Min(const Point3<T>& p1, const Point3<T>& p2) {
      return Point3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
  }

  template <typename T> inline Point3<T>
  Max(const Point3<T>& p1, const Point3<T>& p2) {
      return Point3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
  }

  template <typename T> inline Point3<T>
    Floor(const Point3<T>& p) {
      return Point3<T>(std::floor(p.x), std::floor(p.y), std::floor(p.z));
  }

  template <typename T> inline Point3<T>
    Ceil(const Point3<T>& p) {
      return Point3<T>(std::ceil(p.x), std::ceil(p.y), std::ceil(p.z));
  }

  template <typename T> inline Point3<T>
    Abs(const Point3<T>& p) {
      return Point3<T>(std::abs(p.x), std::abs(p.y), std::abs(p.z));
  }

  template <typename T> inline Point3<T>
  Permute(const Point3<T>& p, int x, int y, int z) {
      return Point3<T>(p[x], p[y], p[z]);
  }

  typedef Point2<int> Point2i;
  typedef Point2<float> Point2f;
  typedef Point3<int> Point3i;
  typedef Point3<float> Point3f;



  template <typename T> class Bounds2 {

  public:
    Bounds2() {
      T minNum = std::numeric_limits<T>::lowest();
      T maxNum = std::numeric_limits<T>::max();
      pMin = Point2<T>(maxNum, maxNum);
      pMax = Point2<T>(minNum, minNum);
    }
    Bounds2(const Point2<T>& p1, const Point2<T>& p2) {
      pMin = Point2<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
      pMax = Point2<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
    }
    template <typename U>
      explicit operator Bounds2<U>() const {
      return Bounds2<U>((Point2<U>)pMin, (Point2<U>)pMax);
    }
    Vector2<T> Diagonal() { return pMax - pMin; }

    T Area() const {
      Vector2<T> d(pMax - pMin);
      return (d.x * d.y);
    }

    Point2<T> pMin, pMax;
  };

  template <typename T> Bounds2<T>
    Intersect(const Bounds2<T>& b1, const Bounds2<T>& b2) {
    return Bounds2<T>(Point2<T>(std::max(b1.pMin.x, b2.pMin.x),
				std::max(b1.pMin.y, b2.pMin.y)),
		      Point2<T>(std::min(b1.pMax.x, b2.pMax.x),
				std::min(b1.pMax.y, b2.pMax.y)));
  }

  typedef Bounds2<int> Bounds2i;
  typedef Bounds2<float> Bounds2f;

}

#endif//GEOMETRY_H
