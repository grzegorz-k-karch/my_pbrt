#include <vector>
#include <string>
#include <iostream>
#include <memory> // gkk
#include "pbrt.h"
#include "api.h"
#include "transform.h" // gkk
#include "medium.h" // gkk
#include "spectrum.h" // gkk
#include "scene.h" // gkk
#include "point.h" // gkk
#include "perspective.h" // gkk
#include "filter.h" // gkk
#include "integrator.h"
#include "../integrators/directlighting.h"
#include "texture.h" // gkk
#include "../textures/constant.h" // gkk
#include "../filters/box.h" // gkk
#include "primitive.h" // gkk
#include "light.h" // gkk
#include "../lights/point.h" // gkk
#include "../shapes/triangle.h" // gkk
#include "../accelerators/bvh.h" // gkk
#include "../materials/matte.h" // gkk


using namespace pbrt;

// gkk
Scene* buildScene() {

  // geometry----------------------------------------------------------------
  Transform objectToWorld = Transform();
  Transform worldToObject = Transform();
  int nTriangles = 4;
  int *vertexIndices = new int[nTriangles*3];
  vertexIndices[0] = 0;
  vertexIndices[1] = 1;
  vertexIndices[2] = 2;
  vertexIndices[3] = 0;
  vertexIndices[4] = 2;
  vertexIndices[5] = 3;
  vertexIndices[6] = 0;
  vertexIndices[7] = 3;
  vertexIndices[8] = 1;
  vertexIndices[9] = 3;
  vertexIndices[10] = 2;
  vertexIndices[11] = 1;

  int nVertices = 4;
  Point3f *P = new Point3f[nVertices];
  P[0] = Point3f(0,0,0);
  P[1] = Point3f(1,0,0);
  P[2] = Point3f(0,1,0);
  P[3] = Point3f(0,0,1);
  Normal3f *N = new Normal3f[nVertices];
  N[0] = Normal3f(0.577,0.577,0.577);
  N[1] = Normal3f(1,0,0);
  N[2] = Normal3f(0,1,0);
  N[3] = Normal3f(0,0,1);

  std::shared_ptr<TriangleMesh> mesh(new TriangleMesh(objectToWorld, nTriangles, vertexIndices,
      nVertices, P, nullptr, N, nullptr, nullptr));

  std::shared_ptr<Triangle> triangle(new Triangle(&objectToWorld, &worldToObject,
      false, mesh, nTriangles));
  //~geometry----------------------------------------------------------------

  // material----------------------------------------------------------------
  Float rgb[3] = {.5, .3, .8};
  std::shared_ptr<ConstantTexture<RGBSpectrum>> Kd
        (new ConstantTexture<RGBSpectrum>(RGBSpectrum::FromRGB(rgb)));
  Float s = 0.f; // if 0 then LambertianReflection BRDF
  std::shared_ptr<ConstantTexture<Float>> Ks
          (new ConstantTexture<Float>(s));
  std::shared_ptr<MatteMaterial> material(new MatteMaterial(Kd, Ks, nullptr));
  //~material----------------------------------------------------------------

  // medium interface--------------------------------------------------------
  MediumInterface mediumiface;
  //~medium interface--------------------------------------------------------

  std::vector<std::shared_ptr<Primitive>> p;
  std::shared_ptr<GeometricPrimitive> triangleMesh(
      new GeometricPrimitive(triangle, material, nullptr, mediumiface));
  p.push_back(triangleMesh);


  Transform lightToWorld = Translate(Vector3f(1,2,1));
  MediumInterface mediumIface;
  Spectrum I(10.0f);
  std::shared_ptr<PointLight> light(new PointLight(lightToWorld, mediumIface, I));
  std::vector<std::shared_ptr<Light>> lights;
  lights.push_back(light);

  Transform *cameraToWorldTr = new Transform(LookAt(Point3f(1,1,-1), Point3f(0,0,0), Vector3f(0,1,0)));
  AnimatedTransform cameraToWorld =
      AnimatedTransform(cameraToWorldTr, 0, cameraToWorldTr, 1);

  Bounds2f screenWindow = Bounds2f(Point2f(-1.66666663, -1), Point2f(1.66666663, 1));
  Float shutterOpen = 0, shutterClose = 1;
  Float lensRadius = 0.0f, focalDistance = 1000000.0f, fov=45.0f;
  Bounds2f cropWindow = Bounds2f(Point2f(0,0), Point2f(1,1));
  Film *film = new Film(Point2i(450,270), cropWindow,
      std::unique_ptr<Filter>(new BoxFilter(Vector2f(0.5f, 0.5f))),
      0.035f, std::string("room-mySimple.exr"), 1);
  Medium *medium = nullptr;
  PerspectiveCamera camera = PerspectiveCamera(cameraToWorld, screenWindow,
      shutterOpen, shutterClose, lensRadius, focalDistance, fov, film, medium);


  std::shared_ptr<BVHAccel> aggregate(new BVHAccel(p, 12, BVHAccel::SplitMethod::HLBVH));
  Scene *scene = new Scene(aggregate, lights);

  return scene;
}

// <main program>
int main(int argc, char* argv[]) {

  // gkk
//  if (argc != 3) {
//    return 0;
//  }
  
//  Options options;
//  std::vector<std::string> filenames;
//  filenames.push_back(argv[1]); // gkk
  
//  // process command line arguments
//  options.imageFile = argv[2]; // gkk
//
//  // pbrtInit();
//  PbrtOptions = options; // gkk
  
  // gkk---------------------------------------------------------------------
  Scene *scene = buildScene();
  std::cout << "yellow" << std::endl;

  std::shared_ptr<DirectLightingIntegrator> directli(new DirectLightingIntegrator());
  directli->Render(*scene);
  //~gkk---------------------------------------------------------------------

//  // <process scene description>
//  if (filenames.size() == 0) {
//	  // <parse scene from standard input>
////      ParseFile("-");
//  }
//  else {
//	  // <parse scene from input files>
//      for (const std::string &f : filenames) {
////          if (!ParseFile(f)) {
////              Error("Couldn't open scene file \"%s\"", f.c_str());
////          }
//      }
//  }
//  // pbrtCleanup();

  return 0;
}
