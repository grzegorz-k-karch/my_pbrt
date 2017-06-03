#include "interaction.h"
#include "shape.h"

namespace pbrt {

SurfaceInteraction::SurfaceInteraction(const Point3f& p, const Vector3f& pError,
                                       const Point2f& uv, const Vector3f& wo,
                                       const Vector3f& dpdu, const Vector3f& dpdv,
                                       const Normal3f& dndu, const Normal3f& dndv,
                                       Float time, const Shape* shape)
: Interaction(p, Normal3f(Normalize(Cross(dpdu, dpdv))), pError, wo, time), // TODO, nullptr),
  uv(uv), dpdu(dpdu), dpdv(dpdv), dndu(dndu), dndv(dndv), shape(shape) {

    shading.n = n;
    shading.dpdu = dpdu;
    shading.dpdv = dpdv;
    shading.dndu = dndu;
    shading.dndv = dndv;

    if (shape && (shape->reverseOrientation ^ shape->transformSwapsHandedness)) {
        n *= -1;
        shading.n *= -1;
    }
}

void SurfaceInteraction::SetShadingGeometry(const Vector3f& dpdus, const Vector3f& dpdvs,
                                            const Normal3f& dndus, const Normal3f& dndvs,
                                            bool orientationIsAuthoritative) {


    shading.n = Normalize((Normal3f)Cross(dpdus, dpdvs));
    if (shape && (shape->reverseOrientation ^ shape->transformSwapsHandedness)) {
        shading.n = -shading.n;
    }
    if (orientationIsAuthoritative) {
        n = Faceforward(n, shading.n);
    }
    else {
        shading.n = Faceforward(shading.n, n);
    }
    shading.dpdu = dpdus;
    shading.dpdv = dpdvs;
    shading.dndu = dndus;
    shading.dndv = dndvs;

}

void SurfaceInteraction::ComputeScatteringFunctions(const RayDifferential& ray,
    MemoryArena& arena, bool allowMultipleLobs, TransportMode mode) {

  ComputeDifferentials(ray);
  primitive->ComputeScatteringFunctions(this, arena, mode, allowMultipleLobs);
}

void SurfaceInteraction::ComputeDifferentials(const RayDifferential& ray) const {

  if (ray.hasDifferentials) {
    // <estimate screen space change in p and (u,v)>
  }
  else {
    dudx = dvdx = 0;
    dudy = dvdy = 0;
    dpdx = dpdy = Vector3f(0, 0, 0);
  }
}

Spectrum SurfaceInteraction::Le(const Vector3f& w) const {

  const AreaLight *area = primitive->GetAreaLight();
//  return area ? area->L(*this, w) : Spectrum(0.0f);
  return Spectrum(0.0f);
}

} // namespace pbrt
