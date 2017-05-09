#ifndef MATERIALS_MATTE_H_
#define MATERIALS_MATTE_H_

#include "pbrt.h"
#include "material.h"
#include "texture.h"
#include <memory>

namespace pbrt {

class MatteMaterial : public Material {
public:
  MatteMaterial(const std::shared_ptr<Texture<Spectrum>>& Kd,
      const std::shared_ptr<Texture<Float>>& sigma,
      const std::shared_ptr<Texture<Float>>& bumpMap)
: Kd(Kd), sigma(sigma), bumpMap(bumpMap) {}

  void ComputeScatteringFunctions(SurfaceInteraction* si, MemoryArena& arena,
      TransportMode mode, bool allowMultipleLobs) const;

private:
  std::shared_ptr<Texture<Spectrum>> Kd;
  std::shared_ptr<Texture<Float>> sigma, bumpMap;
};

} /* namespace pbrt */

#endif /* MATERIALS_MATTE_H_ */
