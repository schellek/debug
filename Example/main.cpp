#include <cstdint>
#include <cstdio>
#include <cinttypes>
#include <string_view>
#include <numeric>

#include "fmt/ostream.hpp"
#include "fmt/container.hpp"
#include "defines.h"
#include "fmt/string_conv.hpp"

fmt::ostream fmt::cout
{
  [](const char *str, fmt::ostream::size_type len) noexcept -> fmt::ostream::size_type
  {
    return static_cast<fmt::ostream::size_type>(fwrite(str, 1U, len, stdout));
  },
  [](void) noexcept -> void
  {
    fflush(stdout);
  }
};

int main()
{
  uint8_t num2 = 192;
  std::array<int, 5> array = {100, 200, 300, 400, 500};
  std::map<int, std::string_view> map;
  std::array<int, 5> nums;

  std::iota(nums.begin(), nums.end(), 1);

  map.emplace(10, "Ten");
  map.emplace(15, "Fifteen");
  map.emplace(5, "Five");

  fmt::cout << "Hello, this is an example!" FMT_ENDL;

  fmt_printf("%-7s", "array:");
  fmt::cout << array << fmt::endl;
  fmt_printf("%-*s", 7, "map:");
  fmt::cout << map << fmt::endl;
  fmt_printf("%*s", -7, "tuple:");
  fmt::cout << std::make_tuple("number + char", 123456789, 'x') << fmt::endl;
  fmt_printf("%*s", -7, "pair:");
  fmt::cout << std::make_pair("array", std::ref(nums)) << fmt::endl;

  PRINTF("printf should also work just %s :%c %" PRIu8 " %p" FMT_ENDL, "fine", ')', num2, nullptr);

  ASSERT(1 != 1);
  LOG("This is never printed" FMT_ENDL);
}