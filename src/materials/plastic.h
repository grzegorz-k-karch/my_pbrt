#ifndef MATERIALS_PLASTIC_H_
#define MATERIALS_PLASTIC_H_

#include "pbrt.h"
#include "material.h"
#include "texture.h"
#include <memory>

namespace pbrt {

class PlasticMaterial : public Material {
public:
  PlasticMaterial(const std::shared_ptr<Texture<Spectrum>>& Kd,
      const std::shared_ptr<Texture<Spectrum>>& Ks,
      const std::shared_ptr<Texture<Float>>& roughness,
      const std::shared_ptr<Texture<Float>>& bumpMap,
      bool remapRoughness)
 : Kd(Kd), Ks(Ks), roughness(roughness), bumpMap(bumpMap),
   remapRoughness(remapRoughness) {}

  void ComputeScatteringFunctions(SurfaceInteraction* si, MemoryArena& arena,
      TransportMode mode, bool allowMultipleLobs) const;

private:
  std::shared_ptr<Texture<Spectrum>> Kd, Ks;
  std::shared_ptr<Texture<Float>> roughness, bumpMap;
  const bool remapRoughness;

};

} /* namespace pbrt */

#endif /* MATERIALS_PLASTIC_H_ */
