#include <vector>
#include <string>
#include <iostream>
#include <memory> // gkk
#include "pbrt.h"
#include "api.h"
#include "transform.h" // gkk
#include "medium.h" // gkk
#include "spectrum.h" // gkk
#include "point.h" // gkk
#include "perspective.h" // gkk
#include "filter.h"
#include "../filters/box.h" // gkk
#include "primitive.h" // gkk
#include "light.h" // gkk
#include "../lights/point.h" // gkk
#include "../shapes/triangle.h"

using namespace pbrt;

// gkk
Scene* buildScene() {

  Transform objectToWorld = Transform();
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
  std::shared_ptr<Point3f> P(new Point3f[nVertices]);
//  TriangleMesh mesh = TriangleMesh(objectToWorld, nTriangles, vertexIndices, nVertices,
//      );

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


  std::shared_ptr<Primitive> aggregate;
//  aggregate->
  Scene *scene;// = new Scene(, );

  return scene;
}

// <main program>
int main(int argc, char* argv[]) {

  // gkk
  if (argc != 3) {
    return 0;
  }
  
  Options options;
  std::vector<std::string> filenames;
  filenames.push_back(argv[1]); // gkk
  
  // process command line arguments  
  options.imageFile = argv[2]; // gkk
  
  // pbrtInit();
  PbrtOptions = options; // gkk
  
  // <process scene description>
  if (filenames.size() == 0) {
	  // <parse scene from standard input>
//      ParseFile("-");
  }
  else {
	  // <parse scene from input files>
      for (const std::string &f : filenames) {
//          if (!ParseFile(f)) {
//              Error("Couldn't open scene file \"%s\"", f.c_str());
//          }
      }
  }
  // pbrtCleanup();

  return 0;
}
