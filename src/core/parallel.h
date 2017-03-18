#ifndef PARALLEL_H
#define PARALLEL_H

#include "pbrt.h"
#include "geometry.h"
#include <functional>
#include <atomic>

namespace pbrt {

void ParallelFor2D(std::function<void(Point2i)> func, const Point2i& count);
void ParallelFor(const std::function<void(int64_t)> &func, int64_t count, int chunkSize = 1);

class AtomicFloat {
public:
  explicit AtomicFloat(Float v = 0) { bits = FloatToBits(v); }
  operator Float() const { return BitsToFloat(bits); }
  Float operator=(Float v) { bits = FloatToBits(v); return v; }
  void Add(Float v) {
    uint32_t oldBits = bits, newBits;
    do {
      newBits = FloatToBits(BitsToFloat(oldBits) + v);
    } while (!bits.compare_exchange_weak(oldBits, newBits));
  }
private:
#if PBRT_FLOAT_AS_DOUBLE
  std::atomic<uint64_t> bits;
#else
  std::atomic<uint32_t> bits;
#endif
};

}

#endif//PARALLEL_H
