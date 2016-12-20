#include "light.h"

namespace pbrt {

Spectrum Light::Le(const RayDifferential& ray) const {
    return Spectrum(0.0f);
}

}
