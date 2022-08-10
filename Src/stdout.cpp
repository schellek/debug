#include <cstdlib>

#include <algorithm>

#include "fmt/stdout.h"
#include "fmt/ostream.hpp"


static char *stringBufferPos, *stringBufferEnd;

static fmt_size_type StringBufferWrite(const char *str, fmt_size_type size) noexcept;

fmt_size_type fmt_write(const char *str, uint16_t len)
{
  return fmt::cout.write(str, len);
}

int fmt_printf(const char *str, ...)
{
  va_list args;
  int retval;

  va_start(args, str);
  retval = fmt::cout.vprintf(str, args);
  va_end(args);

  return retval;
}

int fmt_sprintf(char *buf, const char *str, ...)
{
  stringBufferPos = buf;
  stringBufferEnd = nullptr;

  fmt::ostream sstream{&StringBufferWrite};

  va_list args;
  int retval;

  va_start(args, str);
  retval = sstream.vprintf(str, args);
  va_end(args);

  *stringBufferPos = '\0';

  return retval;
}

int fmt_snprintf(char *buf, size_t n, const char *str, ...)
{
  stringBufferPos = buf;
  stringBufferEnd = buf + n - 1U;

  fmt::ostream sstream{&StringBufferWrite};

  va_list args;
  int retval;

  va_start(args, str);
  retval = sstream.vprintf(str, args);
  va_end(args);

  *stringBufferPos = '\0';

  return retval;
}

int fmt_puts(const char *str)
{
  using namespace std::string_view_literals;

  fmt_size_type len = fmt::cout.write(std::string_view{str});
  len += fmt::cout.write(FMT_ENDL ""sv);

  return static_cast<int>(len);
}

int fmt_putchar(int c)
{
  fmt::cout.write(static_cast<char>(c));
  return static_cast<int>(c);
}

void fmt_flush(void)
{
  fmt::flush(fmt::cout);
}

void fmt_assert_failed(const char *expr, const char *file, uint32_t line)
{
  if (nullptr != expr)
    fmt::cout << "Assertion failed: " << expr << ", file " << file << ", line " << line << fmt::endl;
  else
    fmt::cout << "Assertion failed: file " << file << ", line " << line << fmt::endl;

  exit(EXIT_FAILURE);
}

static fmt_size_type StringBufferWrite(const char *str, fmt_size_type size) noexcept
{
  if (stringBufferEnd == nullptr)
    /* Do nothing */;
  else if ((stringBufferPos + size) > stringBufferEnd)
    size = static_cast<fmt_size_type>(stringBufferEnd - stringBufferPos);

  stringBufferPos = std::copy_n(str, size, stringBufferPos);
  return size;
}
