#pragma once

#include <cstdint>
#include <string_view>

namespace debug
{

constexpr uint16_t g_returnBufferLen = 25U;
inline char g_returnBuffer[g_returnBufferLen];

constexpr char uppercaseHexLookup[16] = 
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

constexpr char lowercaseHexLookup[16] =
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

template<typename _int_t>
std::string_view int2str(_int_t value)
{
  static_assert(std::is_integral_v<_int_t>, "Function parameter needs to be an integral type");

  char digit;
  char *data = g_returnBuffer;
  uint16_t len = 0U;
  bool first = true;
  _int_t startvalue;
  _int_t div;

  if constexpr (std::is_same_v<_int_t, uint8_t> || std::is_same_v<_int_t, int8_t>)
    startvalue = static_cast<_int_t>(100);

  else if constexpr (std::is_same_v<_int_t, uint16_t> || std::is_same_v<_int_t, int16_t>)
    startvalue = static_cast<_int_t>(10000);

  else if constexpr (std::is_same_v<_int_t, uint32_t> || std::is_same_v<_int_t, int32_t>)
    startvalue = static_cast<_int_t>(1000000000);
  
  else if constexpr (std::is_same_v<_int_t, uint64_t>)
    startvalue = static_cast<_int_t>(10000000000000000000U);

  else if constexpr (std::is_same_v<_int_t, int64_t>)
    startvalue = static_cast<_int_t>(1000000000000000000);
  
  else
    startvalue = static_cast<_int_t>(0);

  if (value < 0)
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

  return { data, len };
}

template<typename _int_t>
std::string_view int2hexstr(_int_t value, bool uppercaseLetters)
{
  static_assert(std::is_integral_v<_int_t>, "Function parameter needs to be an integral type");

  const char *const hexLoopup = uppercaseLetters ? uppercaseHexLookup : lowercaseHexLookup;

  uint8_t nibbleValue;
  char *data = g_returnBuffer;
  uint16_t len = 0U;
  bool first = true;
  int8_t pos;

  for (pos = (sizeof(_int_t) * 8) - 4; pos > -1; pos -= 4)
  {
    nibbleValue = (value >> pos) & 0x0000000F;

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

  return { data, len };
}

std::string_view float2string(float value);

}
