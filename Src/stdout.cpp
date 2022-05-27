#include <cstdlib>

#include "fmt/stdout.h"
#include "fmt/ostream.hpp"


void fmt_write(const char *str, uint16_t len)
{
  fmt::cout.write(str, len);
}

void fmt_printf(const char *str, ...)
{
  va_list argList;
  va_start(argList, str);
  fmt::cout.vprintf(str, argList);
  va_end(argList);
}

void fmt_puts(const char *str)
{
  fmt::cout << str << fmt::endl;
}

void fmt_putchar(char c)
{
  fmt::cout << c;
}

void fmt_assert_failed(const char *expr, const char *file, uint32_t line)
{
  if (nullptr != expr)
    fmt_printf("Assertion failed: %s, file %s, line %u32\r\n", expr, file, line);
  else
    fmt_printf("Assertion failed: file %s, line %u32\r\n", file, line);

  exit(EXIT_FAILURE);
}
