#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "geometry.h"
#include "interaction.h"
#include "light.h"
#include "material.h"

#include <memory>

namespace pbrt {

class Shape;

class Primitive {

public:

  virtual Bounds3f WorldBound() const = 0;

  virtual bool Intersect(const Ray& ray, SurfaceInteraction* isect) const = 0;
  virtual bool IntersectP(const Ray& ray) const = 0;

  virtual const AreaLight* GetAreaLight() const = 0;

  virtual const Material* GetMaterial() const = 0;

  virtual void ComputeScatteringFunctions(SurfaceInteraction* isect,
      MemoryArena& arena, TransportMode mode, bool allowMultipleLobes) const = 0;
};

class GeometricPrimitive : public Primitive {

public:
  GeometricPrimitive(const std::shared_ptr<Shape>& shape,
      const std::shared_ptr<Material>& material,
      const std::shared_ptr<AreaLight>& areaLight,
      const MediumInterface& mediumInterface)
: shape(shape), material(material),
  areaLight(areaLight), mediumInterface(mediumInterface) {}

  virtual Bounds3f WorldBound() const;

  virtual bool Intersect(const Ray& ray, SurfaceInteraction* isect) const;
  virtual bool IntersectP(const Ray& ray) const;

  virtual const AreaLight* GetAreaLight() const;

  virtual const Material* GetMaterial() const;

  virtual void ComputeScatteringFunctions(SurfaceInteraction* isect,
      MemoryArena& arena, TransportMode mode, bool allowMultipleLobes) const;

private:

  std::shared_ptr<Shape> shape;
  std::shared_ptr<Material> material;
  std::shared_ptr<AreaLight> areaLight;
  MediumInterface mediumInterface;
};

class TransformedPrimitive : public Primitive {

public:
  TransformedPrimitive(std::shared_ptr<Primitive>& primitive,
      const AnimatedTransform& primitiveToWorld)
: primitive(primitive), primitiveToWorld(primitiveToWorld) {}
private:
  std::shared_ptr<Primitive> primitive;
  const AnimatedTransform primitiveToWorld;
};

}

#endif//PRIMITIVE_H

