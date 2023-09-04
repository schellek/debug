#include <cstdlib>
#include <algorithm>

#include "fmt/ostream.hpp"
#include "fmt/stdout.h"

static char *stringBufferPos, *stringBufferEnd;

static tFmtSize StringBufferWrite(const char *str, tFmtSize size);

tFmtSize FmtWrite(const char *str, tFmtSize len)
{
  return FMT_ABI::cout.write(str, len);
}

int FmtPrintf(const char *str, ...)
{
  va_list args;
  int retval;

  va_start(args, str);
  retval = FMT_ABI::cout.vprintf(str, args);
  va_end(args);

  return retval;
}

int FmtSPrintf(char *buf, const char *str, ...)
{
  stringBufferPos = buf;
  stringBufferEnd = nullptr;

  FMT_ABI::OStream sstream{&StringBufferWrite};

  va_list args;
  int retval;

  va_start(args, str);
  retval = sstream.vprintf(str, args);
  va_end(args);

  *stringBufferPos = '\0';

  return retval;
}

int FmtSNPrintf(char *buf, size_t n, const char *str, ...)
{
  stringBufferPos = buf;
  stringBufferEnd = buf + n - 1u;

  FMT_ABI::OStream sstream{&StringBufferWrite};

  va_list args;
  int retval;

  va_start(args, str);
  retval = sstream.vprintf(str, args);
  va_end(args);

  *stringBufferPos = '\0';

  return retval;
}

int FmtPutS(const char *str)
{
  tFmtSize len = FMT_ABI::cout.write(std::string_view{str});
  len += FMT_ABI::cout.write(FMT_ENDL, static_cast<tFmtSize>(sizeof(FMT_ENDL) - 1u));

  return static_cast<int>(len);
}

int FmtPutChar(int c)
{
  FMT_ABI::cout.write(static_cast<char>(c));
  return static_cast<int>(c);
}

void FmtFlush(void)
{
  FMT_ABI::flush(FMT_ABI::cout);
}

void FmtAssertFailed(const char *expr, const char *file, uint32_t line)
{
  if (nullptr != expr)
    FMT_ABI::cout << "Assertion failed: " << expr << ", file " << file << ", line " << line << FMT_ABI::endl;
  else
    FMT_ABI::cout << "Assertion failed: file " << file << ", line " << line << FMT_ABI::endl;

  exit(EXIT_FAILURE);
}

static tFmtSize StringBufferWrite(const char *str, tFmtSize size)
{
  if (stringBufferEnd == nullptr)
    /* Do nothing */;
  else if ((stringBufferPos + size) > stringBufferEnd)
    size = static_cast<tFmtSize>(stringBufferEnd - stringBufferPos);

  stringBufferPos = std::copy_n(str, size, stringBufferPos);
  return size;
}
