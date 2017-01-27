#ifndef CORE_MATERIAL_H
#define CORE_MATERIAL_H

#include "pbrt.h"

namespace pbrt {

enum class TransportMode { Radiance, Importance };

class Material {
public:
  virtual void ComputeScatteringFunctions(SurfaceInteraction* isect,
      MemoryArena& arena, TransportMode mode, bool allowMultipleLobes) const = 0;
};

} // namespace pbrt

#endif // CORE_MATERIAL_H
