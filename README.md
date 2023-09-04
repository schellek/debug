# fmt

This repository contains a custom implementation of `stdio` and `ostream` without the use of heap-allocations.
Tis library is mainly dedicated to microcontrollers, however they are platform independent.

[![CMake Build](https://github.com/schellek/fmt/actions/workflows/cmake.yml/badge.svg)](https://github.com/schellek/fmt/actions/workflows/cmake.yml)

For example it can be used as follows:
#### cout.cpp
```c++
#include <cstdio>

#include "fmt/ostream.hpp"

Fmt::OStream Fmt::cout
{
  [](const char *str, Fmt::OStream::tSize len) -> Fmt::OStream::tSize
  {
    return static_cast<Fmt::OStream::tSize>(fwrite(str, 1u, len, stdout));
  },
  [](void) -> void
  {
    fflush(stdout);
  }
};
```

#### someFile.c
```c
#include "fmt/stdout.h"

#ifdef DEBUG
#define PRINTF(...)       FmtPrintf(__VA_ARGS__)
#define LOG(STR)          FmtWrite(STR, sizeof(STR) - 1u)
#define ASSERT(EXPR)      (EXPR) ? (void)0 : FmtAssertFailed(#EXPR, __FILE__, __LINE__)

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
  /// Neither "Hello %s" as well as "World" are also not stored in their corresponding section
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
