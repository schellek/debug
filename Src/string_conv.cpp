#include "string_conv.hpp"
#include <cstring>

using namespace std::string_view_literals;

namespace debug
{

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

std::string_view float2string(float value)
{
  const ieee_754_single *number = reinterpret_cast<ieee_754_single *>(&value);
  int8_t exponent = number->b.exponent - 127;
  uint32_t mantissa = (number->b.fraction >> (23U - exponent)) | (1U << exponent);
  char tempBuffer[g_returnBufferLen];
  char *data = tempBuffer;
  uint16_t len = 0U;
  std::string_view retstr;

  if (number->w == 0x7F800000U)
    return "inf"sv;

  if (number->w == 0xFF800000U)
    return "-inf"sv;

  if ((number->w & 0x7F800000) == 0x7F800000)
    return "nan"sv;

  if (number->b.sign == 1U)
    data[len++] = '-';

  if (mantissa != 0U)
    retstr = int2str(mantissa);

  memcpy(reinterpret_cast<void *>(&data[len]), reinterpret_cast<const void *>(retstr.data()), retstr.size());
  len += retstr.size();

  data = g_returnBuffer;
  memcpy(reinterpret_cast<void *>(data), reinterpret_cast<const void *>(tempBuffer), len);

  return std::string_view{ data, len };
}

} // debug