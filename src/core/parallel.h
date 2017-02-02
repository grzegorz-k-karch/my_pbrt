#ifndef PARALLEL_H
#define PARALLEL_H

#include "geometry.h"
#include <functional>

namespace pbrt {

void ParallelFor2D(std::function<void(Point2i)> func, const Point2i& count);
void ParallelFor2D(const std::function<void(int)> &func, int count, int chunkSize);

}

#endif//PARALLEL_H
