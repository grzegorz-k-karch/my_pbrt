#ifndef CORE_RNG_H
#define CORE_RNG_H

#include "pbrt.h"

#include <chrono>
#include <random>
#include <limits>

namespace pbrt {
#ifdef PBRT_FLOAT_IS_DOUBLE
static const Float OneMinusEpsilon = 0x1.fffffffffffffp-1;
#else
static const Float OneMinusEpsilon = 0x1.fffffep-1;
#endif

class RNG {
public:
  RNG()
: distribution(0,std::numeric_limits<uint32_t>::max()) { }
  RNG(uint64_t sequenceIndex)
  : distribution(0,std::numeric_limits<uint32_t>::max()) {
    SetSequence(sequenceIndex);
  }

  void SetSequence(uint64_t sequenceIndex) {
    // TODO
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
  }

  uint32_t UniformUInt32() {
    // TODO (preliminary version currently)
    return distribution(generator);
  }
  uint32_t UniformUInt32(uint32_t b) {
    uint32_t threshold = (~b + 1u)%b;
    while (true) {
      uint32_t r = UniformUInt32();
      if (r >= threshold) {
        return r%b;
      }
    }
  }

  Float UniformFloat() {
    return std::min(OneMinusEpsilon, UniformUInt32()*0x1p-32f);
  }

private:
  std::default_random_engine generator;
  std::uniform_int_distribution<uint32_t> distribution;
};

} // namespace pbrt

#endif//CORE_RNG_H
