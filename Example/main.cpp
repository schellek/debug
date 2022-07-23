#include "fmt/ostream.hpp"
#include "defines.h"
#include <cstdint>
#include <iostream>

fmt::ostream fmt::cout{
  [](const char *str, uint16_t len) noexcept -> uint16_t
  {
    std::cout.write(str, len);
    return len;
  }
};

int main()
{
  uint32_t bitpattern = 0b11000001100100110011001100110011;
  float fnum = *reinterpret_cast<float *>(&bitpattern);

  int32_t num1 = 1234567890;
  uint8_t num2 = 255;
  std::array<uint32_t, 5> array = {0x12345678U, 0xFFFFFFFFU, 0x55555555, 0x00000000, 0x01020304};

  fmt::cout << "Hello, this is an example!\n";
  fmt::print(fmt::cout, "I can also print this: ", num1, " and this: ", num2, fmt::endl);
  fmt::cout << "I can also print this: " << num1 << " and this: " << num2 << fmt::endl;
  PRINTF("%s %c%c %hhu %p" FMT_ENDL, "printf should also work just fine", ';', ')', 192, NULL);

  ASSERT(1 != 1);
  LOG("This is never printed" FMT_ENDL);
}