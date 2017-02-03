#ifndef PARALLEL_H
#define PARALLEL_H

#include "geometry.h"
#include <functional>

namespace pbrt {

void ParallelFor2D(std::function<void(Point2i)> func, const Point2i& count);
void ParallelFor(const std::function<void(int64_t)> &func, int64_t count, int chunkSize = 1);

}

#endif//PARALLEL_H
