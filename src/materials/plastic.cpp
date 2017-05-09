/*
 * plastic.cpp
 *
 *  Created on: May 9, 2017
 *      Author: gkk
 */

#include "plastic.h"
#include "reflection.h"
#include "memory.h"
#include "texture.h"

namespace pbrt {

void PlasticMaterial::ComputeScatteringFunctions(SurfaceInteraction* si, MemoryArena& arena,
    TransportMode mode, bool allowMultipleLobs) const {

  // <perform bump mapping with bumpMap, if present>
  if (bumpMap) {
//    Bump(bumpMap, si); // TODO
  }

  si->bsdf = ARENA_ALLOC(arena, BSDF)(*si);

  // <initialize diffuse component of plastic material>
  Spectrum kd = Kd->Evaluate(*si).Clamp();
  if (!kd.IsBlack()) {
    si->bsdf->Add(ARENA_ALLOC(arena, LambertianReflection)(kd));
  }

  // <initialize specular component of plastic material>
//  Spectrum ks = Ks->Evaluate(*si).Clamp();
//  if (!ks.IsBlack()) {
//    Fresnel *fresnel = ARENA_ALLOC(arena, FresnelDielectric)(1.f, 1.5f);
//    // <create microfacet distribution distrib for plastic material>
//    Float rough = roughness->Evaluate(*si);
//    if (remapRoughness) {
//      rough = TrowbridgeReitzDistribution::RoughnessToAlpha(rough);
//    }
//    MicrofacetDistribution *distrib =
//        ARENA_ALLOC(arena, TrowbridgeReitzDistribution)(rough, rough);
//
//    BxDF *spec = ARENA_ALLOC(arena, MicrofacetReflection)(ks, distrib, fresnel);
//    si->bsdf->Add(spec);
//  }
}

} /* namespace pbrt */
