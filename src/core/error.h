#ifndef CORE_ERROR_H
#define CORE_ERROR_H

#include "pbrt.h"

namespace pbrt {

#ifdef __GNUC__
#define PRINTF_FUNC __attribute__((__format__(__printf__, 1, 2)))
#else
#define PRINTF_FUNC
#endif//__GNUC__

  void Warning(const char *, ...) PRINTF_FUNC;
  void Error(const char*, ...) PRINTF_FUNC;

} // namespace pbrt

#endif//CORE_ERROR_H
