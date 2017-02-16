#include "orthographic.h"

namespace pbrt {

Float OrthographicCamera::GenerateRay(const CameraSample& sample, Ray* ray) const {

  // <compute raster and camera sample positions>
  Point3f pFilm = Point3f(sample.pFilm.x, sample.pFilm.y, 0);
  Point3f pCamera = rasterToCamera(pFilm);

  *ray = Ray(pCamera, Vector3f(0, 0, 1));

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

Float OrthographicCamera::GenerateRayDifferential(const CameraSample& sample,
    RayDifferential* ray) const {

  // <compute main orthographic viewing ray>
  //  <compute raster and camera sample positions>
  Point3f pFilm = Point3f(sample.pFilm.x, sample.pFilm.y, 0);
  Point3f pCamera = rasterToCamera(pFilm);
  *ray = RayDifferential(pCamera, Vector3f(0, 0, 1));

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

  // <compute ray differentials for OrthographicCamera>
  if (lensRadius > 0) {
    // <compute OrthographicCamera ray differentials accounting for lens>
    // <sample points on lens>
    Point2f pLens = lensRadius*ConcentricSampleDisk(sample.pLens);

    // <compute point on plane of focus>
    Float ft = focalDistance/ray->d.z;
    Point3f pFocus = pCamera + dxCamera + (ft*Vector3f(0, 0, 1));
    ray->rxOrigin = Point3f(pLens.x, pLens.y, 0);
    ray->rxDirection = Normalize(pFocus - ray->rxOrigin);

    pFocus = pCamera + dyCamera + (ft*Vector3f(0, 0, 1));
    ray->ryOrigin = Point3f(pLens.x, pLens.y, 0);
    ray->ryDirection = Normalize(pFocus - ray->ryOrigin);

  }
  else {
    ray->rxOrigin = ray->o + dxCamera;
    ray->ryOrigin = ray->o + dyCamera;
    ray->rxDirection = ray->ryDirection = ray->d;
  }

  ray->time = Lerp(sample.time, shutterOpen, shutterClose);
  ray->hasDifferentials = true;
  ray->medium = medium;
  *ray = cameraToWorld(*ray);

  return 1;
}

} // namespace pbrt

