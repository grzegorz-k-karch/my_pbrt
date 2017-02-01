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
  Bounds3f bounds;
  for (int i = start; i < end; ++i) {
    bounds = Union(bounds, primitiveInfo[i].bounds);
  }

  int nPrimitives = end - start;
  if (nPrimitives == 1) {
    // <create leaf BVHBuildNode>
    int firstPrimOffset = orderedPrims.size();
    for (int i = start; i < end; ++i) {
      int primNum = primitiveInfo[i].primitiveNumber;
      orderedPrims.push_back(primitives[primNum]);
    }
    node->InitLeaf(firstPrimOffset, nPrimitives, bounds);
    return node;
  }
  else {
    // <compute bound of primitive centroids, choose split dimension dim>
    Bounds3f centroidBounds;
    for (int i = start; i < end; ++i) {
      centroidBounds = Union(centroidBounds, primitiveInfo[i].centroid);
    }
    int dim = centroidBounds.MaximumExtent();

    // <partition primitives into two sets and build children>
    int mid = (start + end)/2;
    if (centroidBounds.pMax[dim] == centroidBounds.pMin[dim]) {
      // <create leaf BVHBuildNode>
      int firstPrimOffset = orderedPrims.size();
      for (int i = start; i < end; ++i) {
        int primNum = primitiveInfo[i].primitiveNumber;
        orderedPrims.push_back(primitives[primNum]);
      }
      node->InitLeaf(firstPrimOffset, nPrimitives, bounds);
      return node;
    }
    else {
      // <partition primitives based on splitMethod>
      switch(splitMethod) {
      case SplitMethod::Middle:{
        // <partition primitives through node's midpoint>
        Float pmid = (centroidBounds.pMin[dim] + centroidBounds.pMax[dim])/2;
        BVHPrimitiveInfo *midPtr = std::partition(&primitiveInfo[start], &primitiveInfo[end-1]+1,
            [dim, pmid](const BVHPrimitiveInfo& pi) {
          return pi.centroid[dim] < pmid;
        });
        mid = midPtr - &primitiveInfo[0];
        if (mid != start && mid != end) {
          break;
        }
      }
      case SplitMethod::EqualCounts: {
        // <partition primitives into equally-sized subsets>
        mid = (start + end)/2;
        std::nth_element(&primitiveInfo[start], &primitiveInfo[mid], &primitiveInfo[end-1]+1,
            [dim](const BVHPrimitiveInfo& a, const BVHPrimitiveInfo& b) {
          return a.centroid[dim] < b.centroid[dim];
        });
        break;
      }
      case SplitMethod::SAH:
      default: {
        // <partition primitives using approximate SAH>
        if (nPrimitives <= 4) {
          // <partition primitives into equally sized subsets>
          mid = (start + end)/2;
          std::nth_element(&primitiveInfo[start], &primitiveInfo[mid], &primitiveInfo[end-1]+1,
              [dim](const BVHPrimitiveInfo& a, const BVHPrimitiveInfo& b) {
            return a.centroid[dim] < b.centroid[dim];
          });
        }
        else {
          // <allocate BucketInfo for SAH partition buckets>
          // <initialize BucketInfo for SAH partition buckets>
          // <compute costs for splitting after each bucket>
          // <find bucket to split at that minimizes SAH metric>
          // <either create leaf or split primitives at selected SAH bucket>
        }
        }
      }
      node->InitInterior(dim,
          recursiveBuild(arena, primitiveInfo, start, mid, totalNodes, orderedPrims),
          recursiveBuild(arena, primitiveInfo, mid, end, totalNodes, orderedPrims));
    }
  }
  return node;
}

} // namespace pbrt
