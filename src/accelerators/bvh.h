#ifndef ACCELERATORS_BVH_H
#define ACCELERATORS_BVH_H

#include "pbrt.h"

#include <vector>
#include <memory>

namespace pbrt {

struct BVHBuildNode;
struct BVHPrimitiveInfo;

class BVHAccel {

public:
  enum class SplitMethod {SAH, HLBVH, Middle, EqualCounts};
  BVHAccel(const std::vector<std::shared_ptr<Primitive>>& p,
      int maxPrimsInNode, SplitMethod splitMethod);

  BVHBuildNode* recursiveBuild(MemoryArena& arena,
      std::vector<BVHPrimitiveInfo>& primitiveInfo, int start, int end, int* totalNodes,
      std::vector<std::shared_ptr<Primitive>>& orderedPrims);

  BVHBuildNode* HLBVHBuild(MemoryArena& arena,
      std::vector<BVHPrimitiveInfo>& primitiveInfo, int* totalNodes,
      std::vector<std::shared_ptr<Primitive>>& orderedPrims) const;

private:
  const int maxPrimsInNode;
  const SplitMethod splitMethod;
  std::vector<std::shared_ptr<Primitive>> primitives;
};

} // namespace pbrt

#endif // ACCELERATORS_BVH_H
