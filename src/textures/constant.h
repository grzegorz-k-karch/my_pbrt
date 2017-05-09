#ifndef TEXTURES_CONSTANT_H_
#define TEXTURES_CONSTANT_H_

#include "pbrt.h"
#include "texture.h"

namespace pbrt {

template <typename T>
class ConstantTexture : public Texture<T> {
public:
  ConstantTexture(const T& value) : value(value) {}
  T Evaluate(const SurfaceInteraction &) const {
    return value;
  }
private:
  T value;
};

} /* namespace pbrt */

#endif /* TEXTURES_CONSTANT_H_ */
