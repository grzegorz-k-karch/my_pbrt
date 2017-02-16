#ifndef CAMERAS_ORTHOGRAPHIC_H
#define CAMERAS_ORTHOGRAPHIC_H

#include "pbrt.h"
#include "camera.h"

namespace pbrt {

class OrthographicCamera : public ProjectiveCamera {

public:
  OrthographicCamera(const AnimatedTransform& cameraToWorld, const Bounds2f& screenWindow,
      Float shutterOpen, Float shutterClose, Float lensRadius, Float focalDistance,
      Film* film, const Medium* medium)
: ProjectiveCamera(cameraToWorld, Orthographic(0, 1), screenWindow,
    shutterOpen, shutterClose,
    lensRadius, focalDistance, film, medium) {

    // <compute differential changes in origin for orthographic camera rays>
    dxCamera = rasterToCamera(Vector3f(1, 0, 0));
    dyCamera = rasterToCamera(Vector3f(0, 1, 0));
  }

  virtual Float GenerateRay(const CameraSample& sample, Ray* ray) const;
  virtual Float GenerateRayDifferential(const CameraSample& sample, RayDifferential* ray) const;

private:
  Vector3f dxCamera, dyCamera;
  Float A;
};

} // namespace pbrt

#endif // CAMERAS_ORTHOGRAPHIC_H
