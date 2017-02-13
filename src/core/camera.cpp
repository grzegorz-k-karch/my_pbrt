#include "camera.h"

namespace pbrt {

Camera::Camera(const AnimatedTransform& cameraToWorld, Float shutterOpen, Float shutterClose,
      Film* film, const Medium* medium)
: cameraToWorld(cameraToWorld), shutterOpen(shutterOpen), shutterClose(shutterClose),
  film(film), medium(medium) {

}

Float Camera::GenerateRayDifferential(const CameraSample& sample, RayDifferential* rd) const {

  Float wt = GenerateRay(sample, rd);

  // <find camera ray after shifting one pixel in the x direction>
  CameraSample sshift = sample;
  sshift.pFilm.x++;
  Ray rx;
  Float wtx = GenerateRay(sshift, &rx);
  if (wtx == 0) {
    return 0;
  }
  rd->rxOrigin = rx.o;
  rd->rxDirection = rx.d;

  // <find camera ray after shifting one pixel in the y direction>
  sshift.pFilm.x--;
  sshift.pFilm.y++;
  Ray ry;
  Float wty = GenerateRay(sshift, &ry);
  if (wty == 0) {
    return 0;
  }
  rd->ryOrigin = ry.o;
  rd->ryDirection = ry.d;

  rd->hasDifferentials = true;
  return wt;
}

} // namespace pbrt
