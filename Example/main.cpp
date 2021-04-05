#include "ostream.hpp"
#include "memory.hpp"
#include "defines.h"
#include <cstdint>
#include <iostream>

debug::ostream debug::cout([](const char *p_str, uint16_t len) -> void
{
  std::cout.write(p_str, len);
});

int main()
{
  uint32_t bitpattern = 0b11000001100100110011001100110011;
  float fnum = *reinterpret_cast<float *>(&bitpattern);

  int32_t num1 = 1234567890;
  uint8_t num2 = 255;
  uint32_t array[] = { 0x12345678U, 0xFFFFFFFFU, 0x55555555, 0x00000000, 0x01020304 };

  debug::cout << "Hello, this is an example!";
  debug::cout << " I can also print this: "<< num1 << " and this: " << num2 << debug::endl;
  DEBUG_PRINTF("%s %c%c %u8 %p\r\n", "printf should also work just fine", ';', ')', 192, NULL);

  debug::cout << debug::memory::bin(array);
  debug::cout << debug::memory::hex(array);

  ASSERT(1 != 1);
  DEBUG_LOG("This is never printed\r\n");
}