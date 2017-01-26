#include "primitive.h"
#include "shape.h"

namespace pbrt {

bool GeometricPrimitive::Intersect(const Ray& ray, SurfaceInteraction* isect) const {

  Float tHit;
  if (!shape->Intersect(ray, &tHit, isect)) {
    return false;
  }
  ray.tMax = tHit;
  isect->primitive = this;
  // TODO <initialize SurfaceInteraction::mediumInterface after Shape intersection>
  return true;
}

Bounds3f GeometricPrimitive::WorldBound() const {
  return shape->WorldBound();
}

bool GeometricPrimitive::IntersectP(const Ray& ray) const {
  return shape->IntersectP(ray);
}

const AreaLight* GeometricPrimitive::GetAreaLight() const {
  return areaLight.get();
}

const Material* GeometricPrimitive::GetMaterial() const {
  material.get();
}

void GeometricPrimitive::ComputeScatteringFunctions(SurfaceInteraction* isect,
    MemoryArena& arena, TransportMode mode, bool allowMultipleLobes) const {

  if (material) {
    material->ComputeScatteringFunctions(isect, arena, mode, allowMultipleLobes);
  }
}

} // namespace pbrt
