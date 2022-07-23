#pragma once

#include <cstring>

#include "fmt/string_conv.hpp"


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

constexpr char uppercaseHexLookup[16U] =
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

constexpr char lowercaseHexLookup[16U] =
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

std::string_view int2str(integral auto value)
{
  using _int_t = decltype(value);

  constexpr auto maxAmountDigits = []() constexpr -> uint8_t
  {
    uint8_t amountDigits = 0U;

    if constexpr (std::is_signed_v<_int_t>)
      ++amountDigits;

    for (_int_t i = std::numeric_limits<_int_t>::max(); i > 0; i /= 10)
      ++amountDigits;

    return amountDigits;
  };

  _int_t nextIterValue;
  bool sign = false;

  char *const end = g_returnBuffer + std::integral_constant<uint8_t, maxAmountDigits()>::value;
  char *begin = end;

  if (value < 0)
  {
    sign = true;
    value = -value;
  }
  else
  {
    sign = false;
  }

  do
  {
    nextIterValue = value / 10;
    *(--begin) = (value - (nextIterValue * 10)) + '0';
    value = nextIterValue;
  } while (value);

  if (sign)
    *(--begin) = '-';

  return {begin, static_cast<size_t>(end - begin)};
}

std::string_view int2hexstr(integral auto value, bool uppercaseLetters)
{
  using _int_t = decltype(value);

  const char (&hexLookup)[16U] = uppercaseLetters ? uppercaseHexLookup : lowercaseHexLookup;

  char *const end = g_returnBuffer + ((sizeof(_int_t) * 8U) / 4U);
  char *begin = end;

  do
  {
    *(--begin) = hexLookup[value & 0xF];
    value >>= 4U;
  } while (value);

  return {begin, static_cast<size_t>(end - begin)};
}

std::string_view int2octstr(integral auto value)
{
  using _int_t = decltype(value);

  char *const end = g_returnBuffer + ((sizeof(_int_t) * 8U) / 3U) + 1;
  char *begin = end;

  do
  {
    *(--begin) = (value & 0x7) + '0';
    value >>= 3U;
  } while (value);

  return {begin, static_cast<size_t>(end - begin)};
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
