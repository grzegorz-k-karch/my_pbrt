#ifndef SAMPLER_H
#define SAMPLER_H

#include "geometry.h"
#include <memory>

namespace pbrt {

class Sampler {
public:
    virtual std::unique_ptr<Sampler> Clone(int seed) = 0;

    virtual void StartPixel(const Point2i& p);
    virtual bool StartNextSample();


    const int64_t samplesPerPixel;
protected:
    Point2i currentPixel;
    int64_t currentPixelSampleIndex;
};



}

#endif//SAMPLER_H
