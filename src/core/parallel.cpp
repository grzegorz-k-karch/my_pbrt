#include "parallel.h"
#include "geometry.h"
#include <functional>

namespace pbrt {

void ParallelFor2D(std::function<void(Point2i)> func, const Point2i& count) {

//    if (threads.size() == 0) {
        for (int y = 0; y < count.y; ++y) {
            for (int x = 0; x < count.x; ++x) {
                func(Point2i(x,y));
            }
        }
//    }
}

void ParallelFor(const std::function<void(int64_t)> &func, int64_t count, int chunkSize) {

//  if (PbrtOptions.nThreads == 1 || count < chunkSize) {
    for (int i = 0; i < count; ++i) {
      func(i);
    }
//  }
}

}
