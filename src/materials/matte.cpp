#include "matte.h"
#include "reflection.h"
#include "memory.h"
#include "texture.h"

namespace pbrt {

void MatteMaterial::ComputeScatteringFunctions(SurfaceInteraction* si, MemoryArena& arena,
      TransportMode mode, bool allowMultipleLobs) const {

  // <perform bump mapping with bumpMap, if present>
  if (bumpMap) {
//    Bump(bumpMap, si); // TODO
  }

  // <evaluate textures for MatteMaterial material and allocate BRDF>
  si->bsdf = ARENA_ALLOC(arena, BSDF)(*si);
  Spectrum r = Kd->Evaluate(*si).Clamp();
  Float sig = Clamp(sigma->Evaluate(*si), 0, 90);

  if (!r.IsBlack()) {
    if (sig == 0) {
      si->bsdf->Add(ARENA_ALLOC(arena, LambertianReflection)(r));
    }
    else {
//      si->bsdf->Add(ARENA_ALLOC(arena, OrenNayar)(r, sig)); // TODO
    }
  }
}

} /* namespace pbrt */
