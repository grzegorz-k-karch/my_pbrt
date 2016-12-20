#ifndef PARALLEL_H
#define PARALLEL_H

#include "geometry.h"
#include <functional>

namespace pbrt {

void ParallelFor2D(std::function<void(Point2i)> func, const Point2i& count);

}

#endif//PARALLEL_H
