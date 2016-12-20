#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <limits>
#include <algorithm>

namespace pbrt {


class Ray {

};

class RayDifferential : public Ray {

};

template <typename T> class Vector2 {

public:
    Vector2() { x = y = 0; }
    Vector2(T _x, T _y) : x(_x), y(_y) {}

    T x, y;
//    explicit Vector2(const Vector3<T>& p) : x(p.x), y(p.y) {
//        Assert(!HasNaNs());
//    }
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;

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
