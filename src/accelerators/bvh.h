#ifndef ACCELERATORS_BVH_H
#define ACCELERATORS_BVH_H

#include "pbrt.h"
#include "primitive.h"

#include <vector>
#include <memory>
#include <atomic>

namespace pbrt {

struct BVHBuildNode;
struct BVHPrimitiveInfo;
struct MortonPrimitive;
struct LinearBVHNode;
 
class BVHAccel : public Aggregate {

public:
  enum class SplitMethod {SAH, HLBVH, Middle, EqualCounts};

  BVHAccel(const std::vector<std::shared_ptr<Primitive>>& p,
      int maxPrimsInNode, SplitMethod splitMethod);

  Bounds3f WorldBound() const;
  ~BVHAccel();
  
  BVHBuildNode* recursiveBuild(MemoryArena& arena,
      std::vector<BVHPrimitiveInfo>& primitiveInfo, int start, int end, int* totalNodes,
      std::vector<std::shared_ptr<Primitive>>& orderedPrims);

  BVHBuildNode* HLBVHBuild(MemoryArena& arena,
      std::vector<BVHPrimitiveInfo>& primitiveInfo, int* totalNodes,
      std::vector<std::shared_ptr<Primitive>>& orderedPrims) const;

  BVHBuildNode* emitLBVH(BVHBuildNode *&buildNodes,
      const std::vector<BVHPrimitiveInfo>& primitiveInfo, MortonPrimitive* mortonPrims,
      int nPrimitives, int* totalNodes, std::vector<std::shared_ptr<Primitive>>& orderedPrims,
      std::atomic<int>* orderedPrimsOffset, int bitIndex) const;

  BVHBuildNode* buildUpperSAH(MemoryArena& arena, std::vector<BVHBuildNode*>& treeletRoots,
      int start, int end, int* totalNodes) const;

  int flattenBVHTree(BVHBuildNode* node, int* offset);

  bool Intersect(const Ray& ray, SurfaceInteraction* isect) const;
  bool IntersectP(const Ray& ray) const;

private:
  const int maxPrimsInNode;
  const SplitMethod splitMethod;
  std::vector<std::shared_ptr<Primitive>> primitives;
  LinearBVHNode *nodes = nullptr;
};

} // namespace pbrt

#endif // ACCELERATORS_BVH_H
