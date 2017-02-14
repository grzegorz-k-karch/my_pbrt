#ifndef CAMERA_H
#define CAMERA_H

#include "pbrt.h"
#include "transform.h"
#include "film.h"

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

class ProjectiveCamera : public Camera {
public:
  ProjectiveCamera(const AnimatedTransform& cameraToWorld, const Transform& cameraToScreen,
      const Bounds2f& screenWindow, Float shutterOpen, Float shutterClose, Float lensr,
      Float focald, Film* film, const Medium* medium)
: Camera(cameraToWorld, shutterOpen, shutterClose, film, medium),
  cameraToScreen(cameraToScreen) {

    // <initialize depth of field parameters>


    // <compute projective camera transformations>
    //  <compute projective camera screen transformations>
    screenToRaster = Scale(film->fullResolution.x, film->fullResolution.y, 1)*
        Scale(1/(screenWindow.pMax.x - screenWindow.pMin.x),
              1/(screenWindow.pMax.y - screenWindow.pMin.y), 1)*
              Translate(Vector3f(-screenWindow.pMin.x, -screenWindow.pMin.y, 0));
    rasterToScreen = Inverse(screenToRaster);

    rasterToCamera = Inverse(cameraToScreen)*rasterToScreen;

  }
protected:
  Transform cameraToScreen, rasterToCamera;
  Transform screenToRaster, rasterToScreen;
};


}

#endif//CAMERA_H
