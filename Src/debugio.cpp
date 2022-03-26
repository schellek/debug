#include "debugio.h"
#include "ostream.hpp"
#include <cstdlib>

void Debug_Write(const char *str, uint16_t len)
{
  debug::cout.write(str, len);
}

void Debug_Printf(const char *str, ...)
{
  va_list argList;
  va_start(argList, str);
  debug::cout.vprintf(str, argList);
  va_end(argList);
}

void Debug_Puts(const char *str)
{
  debug::cout << str;
}

void Debug_PutChar(char c)
{
  debug::cout << c;
}

void Debug_AssertFailed(const char *expr, const char *file, uint32_t line)
{
  Debug_Printf("Assertion failed: %s, file %s, line %u32\r\n", expr, file, line);
  exit(EXIT_FAILURE);
}

void Debug_HAL_AssertFailed(const char *file, uint32_t line)
{
  Debug_Printf("Assertion failed: file %s, line %u32\r\n", file, line);
  exit(EXIT_FAILURE);
}