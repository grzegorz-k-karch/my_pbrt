#include "perspective.h"

namespace pbrt {

PerspectiveCamera::PerspectiveCamera(const AnimatedTransform& cameraToWorld, const Bounds2f& screenWindow,
    Float shutterOpen, Float shutterClose, Float lensRadius, Float focalDistance,
    Float fov, Film* film, const Medium* medium)
: ProjectiveCamera(cameraToWorld, Perspective(fov, 1e-2f, 1000.0f),
    screenWindow, shutterOpen, shutterClose,
    lensRadius, focalDistance, film, medium) {

  // <compute differential changes in origin for perspective camera rays>
  dxCamera = (rasterToCamera(Point3f(1, 0, 0)) - rasterToCamera(Point3f(0, 0, 0)));
  dyCamera = (rasterToCamera(Point3f(0, 1, 0)) - rasterToCamera(Point3f(0, 0, 0)));

  // <compute image plane bounds at z = 1 for PerspectiveCamera>
  Point2i res = film->fullResolution;
  Point3f pMin = rasterToCamera(Point3f(0, 0, 0));
  Point3f pMax = rasterToCamera(Point3f(res.x, res.y, 0));
  pMin /= pMin.z;
  pMax /= pMax.z;
  A = std::abs(pMax.x - pMin.x)*(pMax.y - pMin.y);
}

Float PerspectiveCamera::GenerateRay(const CameraSample& sample, Ray* ray) const {

  // <compute raster and camera sample positions>
  Point3f pFilm = Point3f(sample.pFilm.x, sample.pFilm.y, 0);
  Point3f pCamera = rasterToCamera(pFilm);

  *ray = Ray(Point3f(0, 0, 0), Normalize(Vector3f(pCamera)));

  // <modify ray for depth of field>
  if (lensRadius > 0) {
    // <sample points on lens>
    Point2f pLens = lensRadius*ConcentricSampleDisk(sample.pLens);

    // <compute point on plane of focus>
    Float ft = focalDistance/ray->d.z;
    Point3f pFocus = (*ray)(ft);

    // <update ray for effect of lens>
    ray->o = Point3f(pLens.x, pLens.y, 0);
    ray->d = Normalize(pFocus - ray->o);
  }

  ray->time = Lerp(sample.time, shutterOpen, shutterClose);
  ray->medium = medium;
  *ray = cameraToWorld(*ray);

  return 1;
}

Float PerspectiveCamera::GenerateRayDifferential(const CameraSample& sample,
    RayDifferential* ray) const {

  // <compute raster and camera sample positions>
  Point3f pFilm = Point3f(sample.pFilm.x, sample.pFilm.y, 0);
  Point3f pCamera = rasterToCamera(pFilm);

  *ray = Ray(Point3f(0, 0, 0), Normalize(Vector3f(pCamera)));

  // <modify ray for depth of field>
  if (lensRadius > 0) {
    // <sample points on lens>
    Point2f pLens = lensRadius*ConcentricSampleDisk(sample.pLens);

    // <compute point on plane of focus>
    Float ft = focalDistance/ray->d.z;
    Point3f pFocus = (*ray)(ft);

    // <update ray for effect of lens>
    ray->o = Point3f(pLens.x, pLens.y, 0);
    ray->d = Normalize(pFocus - ray->o);
  }

  // <compute offset rays for PerspectiveCamera ray differentials>
  if (lensRadius > 0) {
    // <compute PerspectiveCamera ray differentials accounting for lens>
    // <sample points on lens>
    Point2f pLens = lensRadius*ConcentricSampleDisk(sample.pLens);

    Vector3f dx = Normalize(Vector3f(pCamera + dxCamera));
    Float ft = focalDistance/dx.z;
    Point3f pFocus = Point3f(0, 0, 0) + (ft*dx);
    ray->rxOrigin = Point3f(pLens.x, pLens.y, 0);
    ray->rxDirection = Normalize(pFocus - ray->rxOrigin);

    Vector3f dy = Normalize(Vector3f(pCamera + dyCamera));
    Float ft = focalDistance/dy.z;
    Point3f pFocus = Point3f(0, 0, 0) + (ft*dy);
    ray->ryOrigin = Point3f(pLens.x, pLens.y, 0);
    ray->ryDirection = Normalize(pFocus - ray->ryOrigin);

  }
  else {
    ray->rxOrigin = ray->ryOrigin = ray->o;
    ray->rxDirection = Normalize(Vector3f(pCamera) + dxCamera);
    ray->ryDirection = Normalize(Vector3f(pCamera) + dyCamera);
  }

  ray->time = Lerp(sample.time, shutterOpen, shutterClose);
  ray->hasDifferentials = true;
  ray->medium = medium;
  *ray = cameraToWorld(*ray);

  return 1;
}

} // namespace pbrt
