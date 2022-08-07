# fmt

This repository contains a custom implementation of `stdio` and `ostream` without the use of heap-allocations.
These debug files are mainly implemented for microcontrollers, however they are platform independent.

[![CMake Build](https://github.com/schellek/fmt/actions/workflows/cmake.yml/badge.svg)](https://github.com/schellek/fmt/actions/workflows/cmake.yml)

For example it can be used as follows:
#### CMakeFiles.txt
```cmake
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG)
  add_subdirectory(path/to/fmt)
endif()
```

#### cout.cpp
```c++
#include <cstdint>
#include <cstdio>

#include "fmt/ostream.hpp"


fmt::ostream fmt::cout
{
  [](const char *str, fmt::ostream::size_type len) noexcept -> fmt::ostream::size_type
  {
    return static_cast<fmt::ostream::size_type>(fwrite(str, 1U, len, stdout));
  },
  [](void) noexcept -> void
  {
    fflush(stdout);
  }
};
```

#### someFile.c
```c
#include "fmt/stdout.h"

#ifdef DEBUG
#define PRINTF(...)       fmt_printf(__VA_ARGS__)
#define LOG(STR)          fmt_write(STR, sizeof(STR) - 1U)
#define ASSERT(EXPR)      (EXPR) ? (void)0 : fmt_assert_failed(#EXPR, __FILE__, __LINE__)

#else
#define PRINTF(...)
#define LOG(STR)
#define ASSERT(EXPR)

#endif /* DEBUG */

#define ARRAY_LEN 10
static int array[ARRAY_LEN];

void HelloWorld(void)
{
  /// Hello World is only printed, when DEBUG is defined
  /// Neither "Hello %s\r\n" as well as "World" are also not stored in their corresponding section
  /// when DEBUG is not defined
  PRINTF("Hello %s" FMT_ENDL, "World");
  LOG("Hello again!" FMT_ENDL);
}

void SetArrayValue(unsigned int idx, int value)
{
  /// The assert statement below is only checked when DEBUG is defined
  ASSERT(idx < ARRAY_LEN);
  array[idx] = value;
}
```
