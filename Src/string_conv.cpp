#include "string_conv.hpp"
#include <cstring>

namespace debug
{

static constexpr uint16_t g_returnBufferLen = 25U;
static char g_returnBuffer[g_returnBufferLen];

union ieee_754_single
{
  struct
  {
    uint32_t fraction : 23U;
    uint32_t exponent : 8U;
    uint32_t sign     : 1U;
  } b;
  uint32_t w;
};

constexpr char uppercaseHexLookup[16] =
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

constexpr char lowercaseHexLookup[16] =
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

template<typename intx_t, intx_t startvalue>
string int2string(intx_t value)
{
  char digit;
  char *data = g_returnBuffer;
  uint16_t len = 0U;
  bool first = true;
  intx_t div;

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

  return { data, len };
}

template<typename intx_t, uint8_t bytes>
string int2hexstring(intx_t value, bool uppercaseLetters)
{
  const char *const hexLoopup = uppercaseLetters ? uppercaseHexLookup : lowercaseHexLookup;

  uint8_t nibbleValue;
  char *data = g_returnBuffer;
  uint16_t len = 0U;
  bool first = true;
  int8_t pos;

  for (pos = bytes * 8 - 4; pos > -1; pos -= 4)
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

string float2string(float value)
{
  const ieee_754_single *number = reinterpret_cast<ieee_754_single *>(&value);
  int8_t exponent = number->b.exponent - 127;
  uint32_t mantissa = (number->b.fraction >> (23U - exponent)) | (1U << exponent);
  char tempBuffer[g_returnBufferLen];
  char *data = tempBuffer;
  uint16_t len = 0U;
  string retstr;

  if (number->w == 0x7F800000U)
  {
    data = const_cast<char *>("inf");
    len = 3U;
  }
  else if (number->w == 0xFF800000U)
  {
    data = const_cast<char *>("-inf");
    len = 4U;
  }
  else if ((number->w & 0x7F800000) == 0x7F800000)
  {
    data = const_cast<char *>("nan");
    len = 3U;
  }
  else
  {
    if (number->b.sign == 1U)
      data[len++] = '-';

    if (mantissa != 0U)
    {
      retstr = uint32_t2string(mantissa);
    }
    memcpy(reinterpret_cast<void *>(&data[len]), reinterpret_cast<const void *>(retstr.data), retstr.len);
    len += retstr.len;
  }

  data = g_returnBuffer;
  memcpy(reinterpret_cast<void *>(data), reinterpret_cast<const void *>(tempBuffer), len);

  return { data, len };
}

string (&uint8_t2string)(uint8_t value)                             = int2string<uint8_t,  100U>;
string (&uint16_t2string)(uint16_t value)                           = int2string<uint16_t, 10000U>;
string (&uint32_t2string)(uint32_t value)                           = int2string<uint32_t, 1000000000U>;
string (&uint64_t2string)(uint64_t value)                           = int2string<uint64_t, 10000000000000000000U>;
string (&int8_t2string)(int8_t value)                               = int2string<int8_t,   100>;
string (&int16_t2string)(int16_t value)                             = int2string<int16_t,  10000>;
string (&int32_t2string)(int32_t value)                             = int2string<int32_t,  1000000000>;
string (&int64_t2string)(int64_t value)                             = int2string<int64_t,  1000000000000000000>;

string (&uint8_t2hexstring)(uint8_t value,   bool uppercaseLetters) = int2hexstring<uint8_t,  1U>;
string (&uint16_t2hexstring)(uint16_t value, bool uppercaseLetters) = int2hexstring<uint16_t, 2U>;
string (&uint32_t2hexstring)(uint32_t value, bool uppercaseLetters) = int2hexstring<uint32_t, 4U>;
string (&uint64_t2hexstring)(uint64_t value, bool uppercaseLetters) = int2hexstring<uint64_t, 8U>;
string (&int8_t2hexstring)(int8_t value,     bool uppercaseLetters) = int2hexstring<int8_t,   1U>;
string (&int16_t2hexstring)(int16_t value,   bool uppercaseLetters) = int2hexstring<int16_t,  2U>;
string (&int32_t2hexstring)(int32_t value,   bool uppercaseLetters) = int2hexstring<int32_t,  4U>;
string (&int64_t2hexstring)(int64_t value,   bool uppercaseLetters) = int2hexstring<int64_t,  8U>;

} // debug