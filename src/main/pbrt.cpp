#include <vector>
#include <string>

// <main program>
int main(int argc, char* argv[]) {

  // Options options;
  std::vector<std::string> filenames;
  // ... process command line arguments
  // pbrtInit();
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
