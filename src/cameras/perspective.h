#ifndef CAMERAS_PERSPECTIVE_H
#define CAMERAS_PERSPECTIVE_H

#include "pbrt.h"
#include "camera.h"

namespace pbrt {

class PerspectiveCamera : public ProjectiveCamera {

public:
  PerspectiveCamera(const AnimatedTransform& cameraToWorld, const Bounds2f& screenWindow,
      Float shutterOpen, Float shutterClose, Float lensRadius, Float focalDistance,
      Float fov, Film* film, const Medium* medium);

  virtual Float GenerateRay(const CameraSample& sample, Ray* ray) const;
  virtual Float GenerateRayDifferential(const CameraSample& sample, RayDifferential* ray) const;

private:
  Vector3f dxCamera, dyCamera;
};

} // namespace pbrt

#endif // CAMERAS_PERSPECTIVE_H
