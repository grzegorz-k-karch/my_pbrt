#ifndef CORE_TEXTURE_H
#define CORE_TEXTURE_H

#include "pbrt.h"

namespace pbrt {

template <typename T>
class Texture {
public:
  virtual T Evaluate(const SurfaceInteraction &) const = 0;
};

} // namespace pbrt

#endif // CORE_TEXTURE_H
