#include "error.h"

#include <stdarg.h>
#include <string>
#include <cstring>
#include <mutex>

namespace pbrt {

  template <typename... Args>
  static std::string StringVaprintf(const std::string& fmt, va_list args) {

    // compute required space (+ extra space for '\0')
    va_list argsCopy;
    va_copy(argsCopy, args);
    size_t size = vsnprintf(nullptr, 0, fmt.c_str(), args) + 1;

    std::string str;
    str.resize(size);
    vsnprintf(&str[0], size, fmt.c_str(), argsCopy);
    str.pop_back(); // remove trailing null
    
    return str;
  }

  void processError(const char* format, va_list args, const char* errorType) {

    std::string errorString;

    // extern int line_num;
    int line_num = 0; // TODO: line_num defined in pbrtparse.y
    if (line_num != 0) {
      // TODO
    }

    errorString += StringVaprintf(format, args);

    // print the error message ( <= 1 time)
    static std::string lastError;
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    if (errorString != lastError) {
      if (!strcmp(errorType, "Warning")) {
	// TODO
      }
      else {
	// LOG(ERROR) << errorString;
      }
      lastError = errorString;
    }
  }

  void Error(const char* format, ...) {

    va_list args;
    va_start(args, format);
    processError(format, args, "Error");
    va_end(args);
  }
  
} // namespace pbrt
