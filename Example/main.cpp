#include <cstdint>
#include <cstdio>
#include <cinttypes>

#include "fmt/ostream.hpp"
#include "fmt/container.hpp"
#include "defines.h"


fmt::ostream fmt::cout
{
  [](const char *str, fmt_size_type len) noexcept -> fmt_size_type
  {
    return static_cast<fmt_size_type>(fwrite(str, 1, len, stdout));
  },
  [](void) noexcept -> void
  {
    fflush(stdout);
  }
};

int main()
{
  int32_t num1 = 1234567890;
  uint8_t num2 = 192;
  std::array<int, 5> array = {100, 200, 300, 400, 500};
  std::map<int, std::string> map;

  map[10] = "Ten";
  map[15] = "Fifteen";
  map[5] = "Five";

  fmt::cout << "Hello, this is an example!" FMT_ENDL;

  fmt_printf("%-7s", "array:");    fmt::cout << array << fmt::endl;
  fmt_printf("%-*s", 7, "map:");   fmt::cout << map << fmt::endl;
  fmt_printf("%*s", -7, "tuple:"); fmt::cout << std::make_tuple(num1, 'r', "str") << fmt::endl;

  PRINTF("printf should also work just %s :%c %" PRIu8 " %p" FMT_ENDL, "fine", ')', num2, nullptr);

  ASSERT(1 != 1);
  LOG("This is never printed" FMT_ENDL);
}