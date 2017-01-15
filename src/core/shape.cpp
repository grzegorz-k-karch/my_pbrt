#include "shape.h"

namespace pbrt {

Shape::Shape(const Transform* objectToWorld,
             const Transform* worldToObject,
             bool reverseOrientation)
: objectToWorld(objectToWorld), worldToObject(worldToObject),
  reverseOrientation(reverseOrientation),
  transformSwapsHandedness(objectToWorld->SwapsHandedness()) {

}

Bounds3f Shape::WorldBound() const {
    return (*objectToWorld)(ObjectBound());
}

} /* namespace pbrt */
