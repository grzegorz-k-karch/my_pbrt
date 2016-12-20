#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "geometry.h"
#include "interaction.h"

namespace pbrt {

class Primitive {

public:
    bool Intersect(const Ray& ray, SurfaceInteraction* isect) const { return false; }
    bool IntersectP(const Ray& ray) const { return false; }
};

}

#endif//PRIMITIVE_H
