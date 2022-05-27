# fmt

This repository contains a lightweigt implementation of `stdio` and `ostream` without the use of heap-allocations.
These debug files are mainly implemented for microcontrollers, however they are platform independent.

[![CMake Build](https://github.com/schellek/fmt/actions/workflows/cmake.yml/badge.svg)](https://github.com/schellek/fmt/actions/workflows/cmake.yml)

For example it can be used as follows:
#### CMakeFiles.txt
```cmake
IF (${CMAKE_BUILD_TYPE} MATCHES Debug)
  add_definitions(-DDEBUG)
  add_subdirectory(path/to/fmt)
ENDIF()
```

#### debug.cpp
```c++
#include <cstdint>
#include <ostream>

#ifdef DEBUG
fmt::ostream fmt::cout([](const char *str, uint16_t len) -> void
{
  std::cout.write(str, len);
});
#endif
```

#### someFile.c
```c
#include "fmt/stdout.h"

#ifdef DEBUG
#define PRINTF(...)       fmt_printf(__VA_ARGS__)
#define LOG(__STR__)      fmt_write(__STR__, sizeof(__STR__) - 1U)
#define ASSERT(__EXPR__)  (__EXPR__) ? (void)0 : fmt_assert_failed(#__EXPR__, __FILE__, __LINE__)

#else
#define PRINTF(...)
#define LOG(__STR__)
#define ASSERT(__EXPR__)

#endif /* DEBUG */

#define ARRAY_LEN 10
static int array[ARRAY_LEN];

void HelloWorld(void)
{
  /* Hello World is only printed, when DEBUG is defined                                           */
  /* Neither "Hello %s\r\n" as well as "World" are also not stored in their corresponding section *
   * when DEBUG is not defined                                                                    */
  PRINTF("Hello %s\r\n", "World");
  LOG("Hello again!\r\n");
}

void SetArrayValue(unsigned int idx, int value)
{
  /* The assert statement below is only checked when DEBUG is defined */
  ASSERT(idx < ARRAY_LEN);
  array[idx] = value;
}
```
