#include "debugio.h"
#include "ostream.hpp"
#include <cstdlib>

void Debug_Write(const char *p_str, uint16_t len)
{
  debug::cout.Write(p_str, len);
}

void Debug_Printf(const char *p_str, ...)
{
  va_list argList;
  va_start(argList, p_str);
  debug::cout.vprintf(p_str, argList);
  va_end(argList);
}

void Debug_Puts(const char *p_str)
{
  debug::cout << p_str;
}

void Debug_PutChar(char c)
{
  debug::cout << c;
}

void Debug_AssertFailed(const char *sz_expr, const char *sz_file, uint32_t line)
{
  Debug_Printf("Assertion failed: %s, file %s, line %u32\r\n", sz_expr, sz_file, line);
  exit(EXIT_FAILURE);
}

void Debug_HAL_AssertFailed(const char *sz_file, uint32_t line)
{
  Debug_Printf("Assertion failed: file %s, line %u32\r\n", sz_file, line);
  exit(EXIT_FAILURE);
}