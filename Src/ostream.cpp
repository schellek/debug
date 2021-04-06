#include "ostream.hpp"
#include "string_conv.hpp"
#include <cstring>
#include <cstdlib>

namespace debug
{

ostream::ostream(void (*write)(const char *p_data, uint16_t len)) : write(*write)
{
  if (write == nullptr)
    exit(EXIT_FAILURE);
}

void ostream::vprintf(const char *p_str, va_list argList)
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
    const void *ptr;
  } arg;

  while (*p_str != '\0')
  {
    if (*p_str != '%')
    {
      *this << *p_str;
      p_str++;
      continue;
    }

    p_str++;
    argType = p_str[0] | (p_str[1] << 8U) | (p_str[2] << 16U);

    if (static_cast<uint16_t>(argType) == u8)
    {
      arg.u32 = va_arg(argList, uint32_t);
      *this << static_cast<uint8_t>(arg.u32);
      p_str += 2U;
    }
    else if (argType == u16)
    {
      arg.u32 = va_arg(argList, uint32_t);
      *this << static_cast<uint16_t>(arg.u32);
      p_str += 3U;
    }
    else if (argType == u32)
    {
      arg.u32 = va_arg(argList, uint32_t);
      *this << arg.u32;
      p_str += 3U;
    }
    else if (argType == u64)
    {
      arg.u64 = va_arg(argList, uint64_t);
      *this << arg.u64;
      p_str += 3U;
    }
    else if (static_cast<uint16_t>(argType) == i8)
    {
      arg.i32 = va_arg(argList, int32_t);
      *this << static_cast<int8_t>(arg.i32);
      p_str += 2U;
    }
    else if (argType == i16)
    {
      arg.i32 = va_arg(argList, int32_t);
      *this << static_cast<int16_t>(arg.i32);
      p_str += 3U;
    }
    else if (argType == i32)
    {
      arg.i32 = va_arg(argList, int32_t);
      *this << arg.i32;
      p_str += 3U;
    }
    else if (argType == i64)
    {
      arg.i64 = va_arg(argList, int64_t);
      *this << arg.i64;
      p_str += 3U;
    }
    else if (static_cast<char>(argType) == 'u')
    {
      arg.u32 = va_arg(argList, uint32_t);
      *this << arg.u32;
      p_str += 1U;
    }
    else if ((static_cast<char>(argType) == 'd') || (static_cast<char>(argType) == 'i'))
    {
      arg.i32 = va_arg(argList, int32_t);
      *this << arg.i32;
      p_str += 1U;
    }
    else if (static_cast<char>(argType) == 'f' || static_cast<char>(argType) == 'F')
    {
      arg.f64 = va_arg(argList, double);
      *this << static_cast<float>(arg.f64);
      p_str += 1U;
    }
    else if (static_cast<char>(argType) == 'c')
    {
      arg.i32 = va_arg(argList, int32_t);
      *this << static_cast<char>(arg.i32);
      p_str += 1U;
    }
    else if (static_cast<char>(argType) == 's')
    {
      arg.ptr = va_arg(argList, const void *);
      *this << reinterpret_cast<const char *>(arg.ptr);
      p_str += 1U;
    }
    else if (static_cast<char>(argType) == 'p')
    {
      arg.ptr = va_arg(argList, void *);
      *this << arg.ptr;
      p_str += 1U;
    }
    else if (static_cast<char>(argType) == 'x')
    {
      arg.u32 = va_arg(argList, uint32_t);
      auto[ data, len ] = uint32_t2hexstring(arg.u32, false);
      write(data, len);
      p_str += 1U;
    }
    else if (static_cast<char>(argType) == 'X')
    {
      arg.u32 = va_arg(argList, uint32_t);
      auto[ data, len ] = uint32_t2hexstring(arg.u32, true);
      write(data, len);
      p_str += 1U;
    }
    else if (static_cast<char>(argType) == 'n')
    {
      arg.ptr = reinterpret_cast<const int *>(va_arg(argList, int32_t *));
      p_str += 1U;
    }
    else if (static_cast<char>(argType) == '%')
    {
      *this << static_cast<char>(argType);
      p_str += 1U;
    }
    else
    {
      p_str += 1U;
    }
  }
}

void ostream::printf(const char *p_str, ...)
{
  va_list argList;
  va_start(argList, p_str);
  vprintf(p_str, argList);
  va_end(argList);
}

ostream& ostream::operator<<(uint8_t value)
{
  auto[ data, len ] = uint8_t2string(value);
  write(data, len);
  return *this;
}

ostream& ostream::operator<<(uint16_t value)
{
  auto[ data, len ] = uint16_t2string(value);
  write(data, len);
  return *this;
}

ostream& ostream::operator<<(uint32_t value)
{
  auto[ data, len ] = uint32_t2string(value);
  write(data, len);
  return *this;
}

ostream& ostream::operator<<(uint64_t value)
{
  auto[ data, len ] = uint64_t2string(value);
  write(data, len);
  return *this;
}

ostream& ostream::operator<<(int8_t value)
{
  auto[ data, len ] = int8_t2string(value);
  write(data, len);
  return *this;
}

ostream& ostream::operator<<(int16_t value)
{
  auto[ data, len ] = int16_t2string(value);
  write(data, len);
  return *this;
}

ostream& ostream::operator<<(int32_t value)
{
  auto[ data, len ] = int32_t2string(value);
  write(data, len);
  return *this;
}

ostream& ostream::operator<<(int64_t value)
{
  auto[ data, len ] = int64_t2string(value);
  write(data, len);
  return *this;
}

ostream& ostream::operator<<(float value)
{
  auto[ data, len ] = float2string(value);
  write(data, len);
  return *this;
}

ostream& ostream::operator<<(char value)
{
  write(&value, 1U);
  return *this;
}

ostream& ostream::operator<<(const char* p_data)
{
  uint16_t len = strlen(p_data);
  write(p_data, len);
  return *this;
}

ostream& ostream::operator<<(const void *ptr)
{
#if UINTPTR_MAX == UINT32_MAX
  auto[ data, len ] = uint32_t2hexstring(reinterpret_cast<uint32_t>(ptr), false);
#elif UINTPTR_MAX == UINT64_MAX
  auto[ data, len ] = uint64_t2hexstring(reinterpret_cast<uint64_t>(ptr), false);
#else
#error "Pointer size couldn't be determined"
#endif

  write("0x", 2U);
  write(data, len);
  return *this;
}

ostream& ostream::operator<<(void (&function)(ostream& stream))
{
  function(*this);
  return *this;
}

void endl(ostream& stream)
{
  stream.write("\r\n", 2U);
}

} // debug