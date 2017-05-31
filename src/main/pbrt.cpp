#include <vector>
#include <string>
#include <iostream>

#include "pbrt.h"
#include "api.h"

using namespace pbrt;

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
