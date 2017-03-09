#include "sampler.h"
#include "geometry.h"
#include "camera.h"

namespace pbrt {

Sampler::Sampler(int64_t samplesPerPixel)
: samplesPerPixel(samplesPerPixel) {}

void Sampler::StartPixel(const Point2i& p) {
	currentPixel = p;
	currentPixelSampleIndex = 0;

	// <reset array offsets for next pixel sample>
	array1DOffset = array2DOffset = 0;
}

bool Sampler::StartNextSample() {
	// <reset array offsets for next pixel sample>
  array1DOffset = array2DOffset = 0;

  return ++currentPixelSampleIndex < samplesPerPixel;
}

bool Sampler::SetSampleNumber(int64_t sampleNum) {
  // <reset array offsets for next pixel sample>
  array1DOffset = array2DOffset = 0;

  currentPixelSampleIndex = sampleNum;
  return currentPixelSampleIndex < samplesPerPixel;
}

CameraSample Sampler::GetCameraSample(const Point2i& pRaster) {

  CameraSample cs;

  cs.pFilm = (Point2f)pRaster + Get2D();
  cs.time = Get1D();
  cs.pLens = Get2D();

  return cs;
}

void Sampler::Request1DArray(int n) {
  samples1DArraySizes.push_back(n);
  sampleArray1D.push_back(std::vector<Float>(n*samplesPerPixel));
}

void Sampler::Request2DArray(int n) {
  samples2DArraySizes.push_back(n);
  sampleArray2D.push_back(std::vector<Point2f>(n*samplesPerPixel));
}

const Float* Sampler::Get1DArray(int n) {
  if (array1DOffset == sampleArray1D.size()) {
    return nullptr;
  }
  return &sampleArray1D[array1DOffset++][currentPixelSampleIndex*n];
}

const Point2f* Sampler::Get2DArray(int n) {
  if (array2DOffset == sampleArray2D.size()) {
    return nullptr;
  }
  return &sampleArray2D[array2DOffset++][currentPixelSampleIndex*n];
}

bool PixelSampler::StartNextSample() {
  current1DDimension = current2DDimension = 0;
  return Sampler::StartNextSample();
}

bool PixelSampler::SetSampleNumber(int64_t sampleNum) {
  current1DDimension = current2DDimension = 0;
  return Sampler::SetSampleNumber(sampleNum);
}

Float PixelSampler::Get1D() {
  if (current1DDimension < samples1D.size()) {
    return samples1D[current1DDimension++][currentPixelSampleIndex];
  }
  else {
    return rng.UniformFloat();
  }
}

Point2f PixelSampler::Get2D() {
  if (current1DDimension < samples1D.size()) {
    return samples2D[current2DDimension++][currentPixelSampleIndex];
  }
  else {
    return Point2f(rng.UniformFloat(), rng.UniformFloat());
  }
}

void GlobalSampler::StartPixel(const Point2i& p) {
  Sampler::StartPixel(p);
  dimension = 0;
  intervalSampleIndex = GetIndexForSample(0);

  // <compute arrayEndDim for dimensions used for array samples>
  arrayEndDim = arrayStartDim + sampleArray1D.size() + 2*sampleArray2D.size();

  // <compute 1D array samples for GlobalSampler>
  for (size_t i = 0; i < samples1DArraySizes.size(); ++i) {
    int nSamples = samples1DArraySizes[i]*samplesPerPixel;
    for (int j = 0; j < nSamples; ++j) {
      int64_t index = GetIndexForSample(j);
      sampleArray1D[i][j] = SampleDimension(index, arrayStartDim + i);
    }
  }

  // <compute 2D array samples for GlobalSampler>
  int dim = arrayStartDim + samples1DArraySizes.size();
  for (size_t i = 0; i < samples2DArraySizes.size(); ++i) {
    int nSamples = samples2DArraySizes[i]*samplesPerPixel;
    for (int j = 0; j < nSamples; ++j) {
      int64_t index = GetIndexForSample(j);
      sampleArray2D[i][j].x = SampleDimension(index, dim);
      sampleArray2D[i][j].y = SampleDimension(index, dim + 1);
    }
    dim += 2;
  }
}

bool GlobalSampler::StartNextSample() {
  dimension = 0;
  intervalSampleIndex = GetIndexForSample(currentPixelSampleIndex + 1);
  return Sampler::StartNextSample();
}

bool GlobalSampler::SetSampleNumber(int64_t sampleNum) {
  dimension = 0;
  intervalSampleIndex = GetIndexForSample(sampleNum);
  return Sampler::SetSampleNumber(sampleNum);
}
Float GlobalSampler::Get1D() {
  if (dimension >= arrayStartDim && dimension < arrayEndDim) {
    dimension = arrayEndDim;
  }
  return SampleDimension(intervalSampleIndex, dimension++);
}

Point2f GlobalSampler::Get2D() {
  if (dimension + 1 >= arrayStartDim && dimension < arrayEndDim) {
    dimension = arrayEndDim;
  }

  Point2f p(SampleDimension(intervalSampleIndex, dimension),
      SampleDimension(intervalSampleIndex, dimension+1));
  dimension += 2;
  return p;
}

}
