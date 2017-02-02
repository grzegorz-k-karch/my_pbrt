#include "bvh.h"
#include "primitive.h"
#include "memory.h"
#include "parallel.h"

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

struct MortonPrimitive {
  int primitiveIndex;
  uint32_t mortonCode;
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
          constexpr int nBuckets = 12;
          struct BucketInfo {
            int count = 0;
            Bounds3f bounds;
          };
          BucketInfo buckets[nBuckets];

          // <initialize BucketInfo for SAH partition buckets>
          for (int i = start; i < end; ++i) {
            int b = nBuckets*centroidBounds.Offset(primitiveInfo[i].centroid)[dim];
            if (b == nBuckets) b = nBuckets - 1;
            buckets[b].count++;
            buckets[b].bounds = Union(buckets[b].bounds, primitiveInfo[i].bounds);
          }

          // <compute costs for splitting after each bucket>
          Float cost[nBuckets-1];
          for (int i = 0; i < nBuckets-1; ++i) {
            Bounds3f b0, b1;
            int count0 = 0, count1 = 0;
            for (int j = 0; j <= i; ++j) {
              b0 = Union(b0, buckets[j].bounds);
              count0 += buckets[j].count;
            }
            for (int j = i+1; j < nBuckets; ++j) {
              b1 = Union(b1, buckets[j].bounds);
              count1 += buckets[j].count;
            }
            cost[i] = .125f +
                (count0*b0.SurfaceArea() + count1*b1.SurfaceArea())/bounds.SurfaceArea();
          }

          // <find bucket to split at that minimizes SAH metric>
          Float minCost = cost[0];
          int minCostSplitBucket = 0;
          for (int i = 1; i < nBuckets-1; ++i) {
            if (cost[i] < minCost) {
              minCost = cost[i];
              minCostSplitBucket = i;
            }
          }

          // <either create leaf or split primitives at selected SAH bucket>
          Float leafCost = nPrimitives;
          if (nPrimitives > maxPrimsInNode || minCost < leafCost) {
            BVHPrimitiveInfo *pmid = std::partition(&primitiveInfo[start], &primitiveInfo[end-1]+1,
                [=](const BVHPrimitiveInfo& pi) {
              int b = nBuckets*centroidBounds.Offset(pi.centroid)[dim];
              if (b == nBuckets) b = nBuckets - 1;
              return b <= minCostSplitBucket;
            });
            mid = pmid - &primitiveInfo[0];
          }
          else {
            // <create leaf BVHBuildNode>
            int firstPrimOffset = orderedPrims.size();
            for (int i = start; i < end; ++i) {
              int primNum = primitiveInfo[i].primitiveNumber;
              orderedPrims.push_back(primitives[primNum]);
            }
            node->InitLeaf(firstPrimOffset, nPrimitives, bounds);
          }
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

inline uint32_t LeftShift3(uint32_t x) {
  if (x == (1 << 10)) --x;
  x = (x | (x << 16)) & 0b00000011000000000000000011111111;
  x = (x | (x <<  8)) & 0b00000011000000001111000000001111;
  x = (x | (x <<  4)) & 0b00000011000011000011000011000011;
  x = (x | (x <<  2)) & 0b00001001001001001001001001001001;
}

inline uint32_t EncodeMorton3(const Vector3f& v) {
  return (LeftShift3(v.z) << 2) | (LeftShift3(v.y) << 1) | (LeftShift3(v.x));
}

BVHBuildNode* BVHAccel::HLBVHBuild(MemoryArena& arena,
    std::vector<BVHPrimitiveInfo>& primitiveInfo, int* totalNodes,
    std::vector<std::shared_ptr<Primitive>>& orderedPrims) const {

  // <compute bounding box of all primitive centroids>
  Bounds3f bounds;
  for (const BVHPrimitiveInfo &pi : primitiveInfo) {
    bounds = Union(bounds, pi.centroid);
  }

  // <compute Morton indices of primitives>
  std::vector<MortonPrimitive> mortonPrims(primitiveInfo.size());
  ParallelFor2D(
      [&](int i) {
    // <initilize mortonPrims[i] for ith primitive>
    constexpr int mortonBits = 10;
    constexpr int mortonScale = 1 << mortonBits;
    mortonPrims[i].primitiveIndex = primitiveInfo[i].primitiveNumber;
    Vector3f centroidOffset = bounds.Offset(primitiveInfo[i].centroid);
    mortonPrims[i].mortonCode = EncodeMorton3(centroidOffset*mortonScale);
  }, primitiveInfo.size(), 512);

  // <radix sort primitive Morton indices>
  RadixSort(&mortonPrims);
  // <create LBVH treelets at bottom of BVH>
  // <create and return SAH BVH from LBVH treelets>
}

} // namespace pbrt
