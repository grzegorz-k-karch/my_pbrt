#ifndef LIGHT_H
#define LIGHT_H

#include "pbrt.h"
#include "geometry.h"

namespace pbrt {

class Light {

public:
    Spectrum Le(const RayDifferential& ray) const;
};

}

#endif//LIGHT_H
