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
//    explicit Vector2(const Vector3<T>& p) : x(p.x), y(p.y) {
//        Assert(!HasNaNs());
//    }
};

template <typename T> inline Vector2<T>
operator*(T s, const Vector2<T>& v) { return v*s; }

template <typename T> inline Vector2<T>
Abs(const Vector2<T>& v) { return Vector2<T>(std::abs(v.x), std::abs(v.y)); }

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

    bool HasNaNs() const {
        return std::isnan(x) || std::isnan(y) || std::isnan(z);
    }

    T x, y, z;
};

template <typename T> inline Vector3<T>
operator*(T s, const Vector3<T>& v) { return v*s; }

template <typename T> inline Vector3<T>
Abs(const Vector3<T>& v) { return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z)); }

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;

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
    T x, y;
//    explicit Point2(const Point3<T>& p) : x(p.x), y(p.y) {
//        Assert(!HasNaNs());
//    }
};

typedef Point2<int> Point2i;



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
    	Vector2<T> d = pMax - pMin;
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
