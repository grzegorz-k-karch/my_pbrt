#include "bvh.h"
#include "primitive.h"
#include "memory.h"

#include <algorithm>

namespace pbrt {

struct BVHPrimitiveInfo {

  BVHPrimitiveInfo() {};
  BVHPrimitiveInfo(size_t primitiveNumber, const Bounds3f& bounds)
  : primitiveNumber(primitiveNumber), bounds(bounds),
    centroid(.5f*bounds.pMin + .5f*bounds.pMax) {}

  size_t primitiveNumber;
  Bounds3f bounds;
  Point3f centroid;
};

struct BVHBuildNode {
  void InitLeaf(int first, int n, const Bounds3f& b) {
    firstPrimOffset = first;
    nPrimitives = n;
    bounds = b;
    children[0] = children[1] = nullptr;
  }
  void InitInterior(int axis, BVHBuildNode* c0, BVHBuildNode* c1) {
    children[0] = c0;
    children[1] = c1;
    bounds = Union(c0->bounds, c1->bounds);
    splitAxis = axis;
    nPrimitives = 0;
  }
  Bounds3f bounds;
  BVHBuildNode *children[2];
  int splitAxis, firstPrimOffset, nPrimitives;
};

BVHAccel::BVHAccel(const std::vector<std::shared_ptr<Primitive>>& p,
    int maxPrimsInNode, SplitMethod splitMethod)
: maxPrimsInNode(std::min(255,maxPrimsInNode)), primitives(p), splitMethod(splitMethod) {

  if (primitives.size() == 0) {
    return;
  }

  // <build BVH from primitives>

  //  <initialize primitiveInfo array for primitives>
  std::vector<BVHPrimitiveInfo> primitiveInfo(primitives.size());
  for (size_t i = 0; i < primitives.size(); ++i) {
    primitiveInfo[i] = {i, primitives[i]->WorldBound()};
  }

  //  <build BVH tree for primitives using primitiveInfo>
  MemoryArena arena(1024*1024);
  int totalNodes = 0;
  std::vector<std::shared_ptr<Primitive>> orderedPrims;
  BVHBuildNode *root;

  if (splitMethod == SplitMethod::HLBVH) {
    // TODO root = HLBVHBuild(arena, primitiveInfo, &totalNodes, orderedPrims);
  }
  else {
    root = recursiveBuild(arena, primitiveInfo, 0, primitives.size(), &totalNodes, orderedPrims);
  }
  primitives.swap(orderedPrims);

  //  <compute representation of depth-first traversal of BVH tree>
}

BVHBuildNode* BVHAccel::recursiveBuild(MemoryArena& arena,
    std::vector<BVHPrimitiveInfo>& primitiveInfo, int start, int end, int* totalNodes,
    std::vector<std::shared_ptr<Primitive>>& orderedPrims) {

  BVHBuildNode *node = arena.Alloc<BVHBuildNode>();
  (*totalNodes)++;

  // <compute bounds of all primitives in BVH node>

  int nPrimitives = end - start;
  if (nPrimitives == 1) {
    // <create leaf BVHBuildNode>
  }
  else {
    // <compute bound of primitive centroids, choose split dimension dim>
    // <partition primitives into two sets and build children>
  }
  return node;
}

} // namespace pbrt
