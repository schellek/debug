#pragma once

#include <cstring>

#include "fmt/string_conv.hpp"
#include "fmt/concepts.hpp"


namespace fmt
{

union ieee_754_single
{
  using int_t = int32_t;
  using uint_t = uint32_t;

  static constexpr uint_t bias = 127U;
  static constexpr uint_t infinity = 0x7F800000;

  struct
  {
    uint_t fraction : 23U;
    uint_t exponent : 8U;
    uint_t sign     : 1U;
  } b;
  uint_t w;
};

union ieee_754_double
{
  using int_t = int64_t;
  using uint_t = uint64_t;

  static constexpr uint_t bias = 1023U;
  static constexpr uint_t infinity = 0x7FF0000000000000;

  struct
  {
    uint_t fraction : 52U;
    uint_t exponent : 11;
    uint_t sign     : 1U;
  } b;
  uint_t w;
};

constexpr uint16_t g_returnBufferLen = 25U;
inline char g_returnBuffer[g_returnBufferLen];

constexpr char uppercaseHexLookup[16] =
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

constexpr char lowercaseHexLookup[16] =
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

std::string_view int2str(integral auto value)
{
  using _int_t = decltype(value);

  char digit;
  char *data = g_returnBuffer;
  uint16_t len = 0U;
  bool first = true;
  _int_t startvalue;
  _int_t div;

  if constexpr (sizeof(_int_t) == 1U)
    startvalue = static_cast<_int_t>(100);

  else if constexpr (sizeof(_int_t) == 2U)
    startvalue = static_cast<_int_t>(10000);

  else if constexpr (sizeof(_int_t) == 4U)
    startvalue = static_cast<_int_t>(1000000000);

  else if constexpr (std::is_same_v<_int_t, uint64_t>)
    startvalue = static_cast<_int_t>(10000000000000000000U);

  else if constexpr (std::is_same_v<_int_t, int64_t>)
    startvalue = static_cast<_int_t>(1000000000000000000);

  else
    startvalue = static_cast<_int_t>(0);

  if constexpr (std::is_unsigned_v<_int_t>)
    /* Do nothing */;

  else if (value < 0)
  {
    data[len++] = '-';
    value = -value;
  }

  for (div = startvalue; div > 0; div /= 10)
  {
    digit = (value % (div * 10)) / div;

    if (digit != 0)
      /* Do nothing */;

    else if (first == true)
      continue;

    first = false;
    data[len++] = digit + '0';
  }

  if (first == true)
  {
    data[0] = '0';
    len = 1U;
  }

  return {data, len};
}

std::string_view int2hexstr(integral auto value, bool uppercaseLetters)
{
  using _int_t = decltype(value);

  const char *const hexLoopup = uppercaseLetters ? uppercaseHexLookup : lowercaseHexLookup;

  uint8_t nibbleValue;
  char *data = g_returnBuffer;
  uint16_t len = 0U;
  bool first = true;
  int8_t pos;

  for (pos = (sizeof(_int_t) * 8) - 4; pos > -1; pos -= 4)
  {
    nibbleValue = (value >> pos) & 0xFU;

    if (nibbleValue != 0U)
      /* Do nothing */;

    else if (first == true)
      continue;

    first = false;
    data[len++] = hexLoopup[nibbleValue];
  }

  if (first == true)
  {
    data[0] = '0';
    len = 1U;
  }

  return {data, len};
}

std::string_view float2string(floating_point auto value)
{
  using namespace std::string_view_literals;

  using _float_t = decltype(value);
  using ieee_754_t = std::conditional_t<std::is_same_v<_float_t, float>, ieee_754_single, ieee_754_double>;
  using uint_t = typename ieee_754_t::uint_t;
  using int_t = typename ieee_754_t::int_t;

  ieee_754_t num = *reinterpret_cast<ieee_754_t *>(&value);

  typename ieee_754_t::int_t exponent = num.b.exponent - ieee_754_t::bias;
  typename ieee_754_t::uint_t mantissa = (num.b.fraction >> (23U - exponent)) | (1U << exponent);
  char tempBuffer[g_returnBufferLen];
  char *data = tempBuffer;
  uint16_t len = 0U;
  std::string_view retstr;

  if ((num.w & ieee_754_t::infinity) == ieee_754_t::infinity)
    return (num.b.sign == 0U) ? "inf"sv : "-inf"sv;

  if ((num.w & 0x7F800000U) == 0x7F800000U)
    return "nan"sv;

  if (num.b.sign == 1U)
    data[len++] = '-';

  if (mantissa != 0U)
    retstr = int2str(mantissa);

  memcpy(reinterpret_cast<void *>(&data[len]), reinterpret_cast<const void *>(retstr.data()), retstr.size());
  len += retstr.size();

  data = g_returnBuffer;
  memcpy(reinterpret_cast<void *>(data), reinterpret_cast<const void *>(tempBuffer), len);

  return std::string_view{data, len};
}

} // namespace fmt
