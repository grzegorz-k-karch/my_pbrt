#ifndef SAMPLER_H
#define SAMPLER_H

#include "pbrt.h"
#include "geometry.h"
#include "rng.h"
#include <memory>

namespace pbrt {

class Sampler {
public:

    Sampler(int64_t samplesPerPixel);
    virtual std::unique_ptr<Sampler> Clone(int seed) = 0;

    virtual void StartPixel(const Point2i& p);
    virtual bool StartNextSample();

    virtual Float Get1D() = 0;
    virtual Point2f Get2D() = 0;

    CameraSample GetCameraSample(const Point2i& pRaster);

    void Request1DArray(int n);
    void Request2DArray(int n);

    virtual int RoundCount(int n) const  {
      return n;
    }

    virtual bool SetSampleNumber(int64_t sampleNum);

    const Float* Get1DArray(int n);
    const Point2f* Get2DArray(int n);
    const int64_t samplesPerPixel;

protected:
    Point2i currentPixel;
    int64_t currentPixelSampleIndex;
    std::vector<int> samples1DArraySizes, samples2DArraySizes;
    std::vector<std::vector<Float>> sampleArray1D;
    std::vector<std::vector<Point2f>> sampleArray2D;

private:
    size_t array1DOffset, array2DOffset;
};


class PixelSampler : public Sampler {
public:
  PixelSampler(int64_t samplesPerPixel, int nSampledDimensions)
: Sampler(samplesPerPixel) {
    for (int i = 0; i < nSampledDimensions; ++i) {
      samples1D.push_back(std::vector<Float>(samplesPerPixel));
      samples2D.push_back(std::vector<Point2f>(samplesPerPixel));
    }
  }

  virtual bool StartNextSample() override;
  virtual bool SetSampleNumber(int64_t sampleNum) override;
  virtual Float Get1D() override;
  virtual Point2f Get2D() override;

protected:
  std::vector<std::vector<Float>> samples1D;
  std::vector<std::vector<Point2f>> samples2D;
  int current1DDimension = 0, current2DDimension = 0;

  RNG rng;
};

class GlobalSampler : public Sampler {
public:
  GlobalSampler(int64_t samplesPerPixel) : Sampler(samplesPerPixel) {}

  virtual int64_t GetIndexForSample(int64_t sampleNum) const = 0;
  virtual Float SampleDimension(int64_t index, int dimension) const = 0;

  virtual void StartPixel(const Point2i& p) override;
  virtual bool StartNextSample() override;
  virtual bool SetSampleNumber(int64_t sampleNum) override;
  virtual Float Get1D() override;
  virtual Point2f Get2D() override;

private:
  int dimension;
  int64_t intervalSampleIndex;
  static const int arrayStartDim = 5;
  int arrayEndDim;
};

}

#endif//SAMPLER_H
