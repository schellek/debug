#include "ostream.hpp"
#include "string_conv.hpp"
#include <cstring>
#include <cstdlib>

using namespace std::string_view_literals;

namespace debug
{

ostream::ostream(void (*_write)(const char *data, uint16_t len)) : _write(*_write)
{
  if (_write == nullptr)
    exit(EXIT_FAILURE);
}

void ostream::write(const char *str, uint16_t len)
{ _write(str, len); }

void ostream::write(std::string_view &str)
{ _write(str.data(), str.size()); }

void ostream::write(std::string_view &&str)
{ _write(str.data(), str.size()); }

void ostream::vprintf(const char *str, va_list argList)
{
  static constexpr uint16_t u8  = static_cast<uint16_t>('u' | ('8' << 8U));
  static constexpr uint32_t u16 = static_cast<uint32_t>('u' | ('1' << 8U) | ('6' << 16U));
  static constexpr uint32_t u32 = static_cast<uint32_t>('u' | ('3' << 8U) | ('2' << 16U));
  static constexpr uint32_t u64 = static_cast<uint32_t>('u' | ('3' << 8U) | ('2' << 16U));
  static constexpr uint16_t i8  = static_cast<uint16_t>('i' | ('8' << 8U));
  static constexpr uint32_t i16 = static_cast<uint32_t>('i' | ('1' << 8U) | ('6' << 16U));
  static constexpr uint32_t i32 = static_cast<uint32_t>('i' | ('3' << 8U) | ('2' << 16U));
  static constexpr uint32_t i64 = static_cast<uint32_t>('i' | ('6' << 8U) | ('4' << 16U));

  uint32_t argType;

  union
  {
    int32_t i32;
    uint32_t u32;
    int64_t i64;
    uint64_t u64;
    double f64;
    const void *addr;
  } arg;

  while (*str != '\0')
  {
    if (*str != '%')
    {
      *this << *str;
      str++;
      continue;
    }

    str++;
    argType = str[0] | (str[1] << 8U) | (str[2] << 16U);

    if (static_cast<uint16_t>(argType) == u8)
    {
      arg.u32 = va_arg(argList, uint32_t);
      *this << static_cast<uint8_t>(arg.u32);
      str += 2U;
    }
    else if (argType == u16)
    {
      arg.u32 = va_arg(argList, uint32_t);
      *this << static_cast<uint16_t>(arg.u32);
      str += 3U;
    }
    else if (argType == u32)
    {
      arg.u32 = va_arg(argList, uint32_t);
      *this << arg.u32;
      str += 3U;
    }
    else if (argType == u64)
    {
      arg.u64 = va_arg(argList, uint64_t);
      *this << arg.u64;
      str += 3U;
    }
    else if (static_cast<uint16_t>(argType) == i8)
    {
      arg.i32 = va_arg(argList, int32_t);
      *this << static_cast<int8_t>(arg.i32);
      str += 2U;
    }
    else if (argType == i16)
    {
      arg.i32 = va_arg(argList, int32_t);
      *this << static_cast<int16_t>(arg.i32);
      str += 3U;
    }
    else if (argType == i32)
    {
      arg.i32 = va_arg(argList, int32_t);
      *this << arg.i32;
      str += 3U;
    }
    else if (argType == i64)
    {
      arg.i64 = va_arg(argList, int64_t);
      *this << arg.i64;
      str += 3U;
    }
    else if (static_cast<char>(argType) == 'u')
    {
      arg.u32 = va_arg(argList, uint32_t);
      *this << arg.u32;
      str += 1U;
    }
    else if ((static_cast<char>(argType) == 'd') || (static_cast<char>(argType) == 'i'))
    {
      arg.i32 = va_arg(argList, int32_t);
      *this << arg.i32;
      str += 1U;
    }
    else if (static_cast<char>(argType) == 'f' || static_cast<char>(argType) == 'F')
    {
      arg.f64 = va_arg(argList, double);
      *this << static_cast<float>(arg.f64);
      str += 1U;
    }
    else if (static_cast<char>(argType) == 'c')
    {
      arg.i32 = va_arg(argList, int32_t);
      *this << static_cast<char>(arg.i32);
      str += 1U;
    }
    else if (static_cast<char>(argType) == 's')
    {
      arg.addr = va_arg(argList, const void *);
      *this << reinterpret_cast<const char *>(arg.addr);
      str += 1U;
    }
    else if (static_cast<char>(argType) == 'p')
    {
      arg.addr = va_arg(argList, void *);
      *this << arg.addr;
      str += 1U;
    }
    else if (static_cast<char>(argType) == 'x')
    {
      arg.u32 = va_arg(argList, uint32_t);
      write(int2hexstr(arg.u32, false));
      str += 1U;
    }
    else if (static_cast<char>(argType) == 'X')
    {
      arg.u32 = va_arg(argList, uint32_t);
      write(int2hexstr(arg.u32, true));
      str += 1U;
    }
    else if (static_cast<char>(argType) == 'n')
    {
      arg.addr = reinterpret_cast<const int *>(va_arg(argList, int32_t *));
      str += 1U;
    }
    else if (static_cast<char>(argType) == '%')
    {
      *this << static_cast<char>(argType);
      str += 1U;
    }
    else
    {
      str += 1U;
    }
  }
}

void ostream::printf(const char *str, ...)
{
  va_list argList;
  va_start(argList, str);
  vprintf(str, argList);
  va_end(argList);
}

ostream& ostream::operator<<(uint8_t value)
{
  write(int2str(value));
  return *this;
}

ostream& ostream::operator<<(uint16_t value)
{
  write(int2str(value));
  return *this;
}

ostream& ostream::operator<<(uint32_t value)
{
  write(int2str(value));
  return *this;
}

ostream& ostream::operator<<(uint64_t value)
{
  write(int2str(value));
  return *this;
}

ostream& ostream::operator<<(int8_t value)
{
  write(int2str(value));
  return *this;
}

ostream& ostream::operator<<(int16_t value)
{
  write(int2str(value));
  return *this;
}

ostream& ostream::operator<<(int32_t value)
{
  write(int2str(value));
  return *this;
}

ostream& ostream::operator<<(int64_t value)
{
  write(int2str(value));
  return *this;
}

ostream& ostream::operator<<(float value)
{
  write(float2string(value));
  return *this;
}

ostream& ostream::operator<<(char value)
{
  write(&value, 1U);
  return *this;
}

ostream& ostream::operator<<(const char* str)
{
  write(std::string_view{ str });
  return *this;
}

ostream& ostream::operator<<(const void *addr)
{
  std::string_view hexstring = int2hexstr(reinterpret_cast<uintptr_t>(addr), false);

  write("0x"sv);
  write(hexstring);
  return *this;
}

ostream& ostream::operator<<(void (&function)(ostream& stream))
{
  function(*this);
  return *this;
}

void endl(ostream& stream)
{
  stream.write("\r\n"sv);
}

} // debug