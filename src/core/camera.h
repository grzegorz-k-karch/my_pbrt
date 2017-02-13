#ifndef CAMERA_H
#define CAMERA_H

#include "pbrt.h"
#include "transform.h"

namespace pbrt {

struct CameraSample {
  Point2f pFilm;
  Point2f pLens;
  Float time;
};

class Camera {

public:
  // methods
  Camera(const AnimatedTransform& cameraToWorld, Float shutterOpen, Float shutterClose,
      Film* film, const Medium* medium);

  virtual Float GenerateRay(const CameraSample& sample, Ray* ray) const = 0;
  virtual Float GenerateRayDifferential(const CameraSample& sample, RayDifferential* rd) const;

  // data
  AnimatedTransform cameraToWorld;
  const Float shutterOpen, shutterClose;
  Film *film;
  const Medium *medium;
};

}

#endif//CAMERA_H
