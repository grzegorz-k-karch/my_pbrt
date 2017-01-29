#ifndef ACCELERATORS_BVH_H
#define ACCELERATORS_BVH_H

#include "pbrt.h"
#include <vector>
#include <memory>

namespace pbrt {

class BVHAccel {

public:
  enum class SplitMethod {SAH, HLBVH, Middle, EqualCounts};
  BVHAccel(const std::vector<std::shared_ptr<Primitive>>& p,
      int maxPrimsInNode, SplitMethod splitMethod);

private:
  const int maxPrimsInNode;
  const SplitMethod splitMethod;
  std::vector<std::shared_ptr<Primitive>> primitives;
};

} // namespace pbrt

#endif // ACCELERATORS_BVH_H
