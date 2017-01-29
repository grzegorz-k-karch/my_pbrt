#include "bvh.h"

#include <algorithm>

namespace pbrt {

BVHAccel::BVHAccel(const std::vector<std::shared_ptr<Primitive>>& p,
    int maxPrimsInNode, SplitMethod splitMethod)
: maxPrimsInNode(std::min(255,maxPrimsInNode)), primitives(p), splitMethod(splitMethod) {

  if (primitives.size() == 0) {
    return;
  }

  // <build BVH from primitives>
}

} // namespace pbrt
