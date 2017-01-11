#ifndef CORE_ERROR_H
#define CORE_ERROR_H

#include "pbrt.h"

namespace pbrt {

#ifdef __GNUC__
#define PRINT_FUNC __attribute__((__format__(__printf__, 1, 2)))
#else
#define PRINT_FUNC
#endif//__GNUC__
  
  void Error(const char*, ...) PRINT_FUNC;

} // namespace pbrt

#endif//CORE_ERROR_H
