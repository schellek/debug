#include <cstdlib>
#include <algorithm>

#include "fmt/ostream.hpp"
#include "fmt/stdout.h"

static char *stringBufferPos, *stringBufferEnd;

static fmt_size_type StringBufferWrite(const char *str, fmt_size_type size) noexcept;

fmt_size_type fmt_write(const char *str, fmt_size_type len)
{
  return FMT_ABI::cout.write(str, len);
}

int fmt_printf(const char *str, ...)
{
  va_list args;
  int retval;

  va_start(args, str);
  retval = FMT_ABI::cout.vprintf(str, args);
  va_end(args);

  return retval;
}

int fmt_sprintf(char *buf, const char *str, ...)
{
  stringBufferPos = buf;
  stringBufferEnd = nullptr;

  FMT_ABI::ostream sstream{&StringBufferWrite};

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

  FMT_ABI::ostream sstream{&StringBufferWrite};

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
  fmt_size_type len = FMT_ABI::cout.write(std::string_view{str});
  len += FMT_ABI::cout.write(FMT_ENDL, static_cast<fmt_size_type>(sizeof(FMT_ENDL) - 1U));

  return static_cast<int>(len);
}

int fmt_putchar(int c)
{
  FMT_ABI::cout.write(static_cast<char>(c));
  return static_cast<int>(c);
}

void fmt_flush(void)
{
  FMT_ABI::flush(FMT_ABI::cout);
}

void fmt_assert_failed(const char *expr, const char *file, uint32_t line)
{
  if (nullptr != expr)
    FMT_ABI::cout << "Assertion failed: " << expr << ", file " << file << ", line " << line << FMT_ABI::endl;
  else
    FMT_ABI::cout << "Assertion failed: file " << file << ", line " << line << FMT_ABI::endl;

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
