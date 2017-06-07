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


using namespace pbrt;

// gkk
Scene* buildScene() {

  Scene *scene = nullptr;

  Transform lightToWorld = Translate(Vector3f(1,2,1));
  MediumInterface mediumIface;
  Spectrum I(10.0f);
  std::shared_ptr<PointLight> light(new PointLight(lightToWorld, mediumIface, I));

  Transform *cameraToWorldTr = new Transform(LookAt(Point3f(1,1,-1), Point3f(0,0,0), Vector3f(0,1,0)));
  AnimatedTransform cameraToWorld =
      AnimatedTransform(cameraToWorldTr, 0, cameraToWorldTr, 1);

  Bounds2f screenWindow = Bounds2f(Point2f(-1.66666663, -1), Point2f(1.66666663, 1));
  Float shutterOpen = 0, shutterClose = 1;
  Float lensRadius = 0.0f, focalDistance = 1000000.0f, fov=45.0f;
  Bounds2f cropWindow = Bounds2f(Point2f(0,0), Point2f(1,1));
  Film film = Film(Point2i(450,270), cropWindow,
      std::unique_ptr<Filter>(new BoxFilter(Vector2f(0.5f, 0.5f))),
      0.035f, std::string("room-mySimple.exr"), 1);
  Medium *medium = nullptr;
  PerspectiveCamera camera = PerspectiveCamera(cameraToWorld, screenWindow,
      shutterOpen, shutterClose, lensRadius, focalDistance, fov, film, medium);

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
