#include <cstdlib>

#include "fmt/stdout.h"
#include "fmt/ostream.hpp"


static_assert(std::is_same_v<ostream_size_type, fmt::ostream::size_type>);

ostream_size_type fmt_write(const char *str, uint16_t len)
{
  return fmt::cout.write(str, len);
}

int fmt_printf(const char *str, ...)
{
  va_list argList;
  int retval;

  va_start(argList, str);
  retval = fmt::cout.vprintf(str, argList);
  va_end(argList);

  return retval;
}

int fmt_puts(const char *str)
{
  using namespace std::string_view_literals;

  ostream_size_type len = fmt::cout.write(std::string_view{str});
  len += fmt::cout.write(FMT_ENDL ""sv);

  return static_cast<int>(len);
}

int fmt_putchar(int c)
{
  fmt::cout.write(static_cast<char>(c));
  return static_cast<int>(c);
}

void fmt_assert_failed(const char *expr, const char *file, uint32_t line)
{
  if (nullptr != expr)
    fmt::print(fmt::cout, "Assertion failed: ", expr, ", file ", file, ", line ", line, fmt::endl);
  else
    fmt::print(fmt::cout, "Assertion failed: file ", file, ", line ", line, fmt::endl);

  exit(EXIT_FAILURE);
}
