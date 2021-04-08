# Debug

This repository contains a lightweigt implementation of `stdio` and `ostream` without the use of heap-allocations.
These debug files are mainly implemented for microcontrollers, however they are platform independent.

[![CMake Build](https://github.com/schellek/debug/actions/workflows/cmake.yml/badge.svg)](https://github.com/schellek/debug/actions/workflows/cmake.yml)

For example it can be used as follows:
#### CMakeFiles.txt
```cmake
IF (${CMAKE_BUILD_TYPE} MATCHES Debug)
  add_definitions(-DDEBUG)
  add_subdirectory(path/to/debug)
ENDIF()
```

#### debug.cpp
```c++
#include <cstdint>

#ifdef DEBUG
debug::ostream debug::cout([](const char *p_str, uint16_t len) -> void
{
  std::cout.write(p_str, len);
});
#endif
```

#### someFile.c
```c
#ifdef DEBUG
#include "debugio.h"
#define DEBUG_PRINTF(...)   Debug_Printf(__VA_ARGS__)
#define DEBUG_LOG(__STR__)  Debug_Write(__STR__, sizeof(__STR__) - 1U)
#define ASSERT(__EXPR__)    (__EXPR__) ? (void)0 : Debug_AssertFailed(#__EXPR__, __FILE__, __LINE__)
#else
#define DEBUG_PRINTF(...)
#define DEBUG_LOG(__STR__)
#define ASSERT(__EXPR__)
#endif

#define ARRAY_LEN 10
static int array[ARRAY_LEN];

void HelloWorld(void)
{
    /* Hello World is only printed, when DEBUG is defined                                           */
    /* Neither "Hello %s\r\n" as well as "World" are also not stored in their corresponding section *
     * when DEBUG is not defined                                                                    */
    DEBUG_PRINTF("Hello %s\r\n", "World");
    DEBUG_LOG("Hello again!\r\n");
}

void SetArrayValue(unsigned int idx, int value)
{
    /* The assert statement below is only checked when DEBUG is defined */
    ASSERT(idx < ARRAY_LEN);
    array[idx] = value;
}
```
