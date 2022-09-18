#include <cstdint>
#include <cstdio>
#include <cinttypes>
#include <string_view>
#include <numeric>

#include "fmt/ostream.hpp"
#include "fmt/container.hpp"
#include "defines.h"
#include "fmt/string_conv.hpp"

Fmt::OStream Fmt::cout
{
  [](const char *str, Fmt::OStream::SizeType len) noexcept -> Fmt::OStream::SizeType
  {
    return static_cast<Fmt::OStream::SizeType>(fwrite(str, 1U, len, stdout));
  },
  [](void) noexcept -> void
  {
    fflush(stdout);
  }
};

int main()
{
  [[maybe_unused]] uint8_t num2 = 192;
  std::array<int, 5> array = {100, 200, 300, 400, 500};
  std::map<int, std::string_view> map;
  std::array<int, 5> nums;

  std::iota(nums.begin(), nums.end(), 1);

  map.emplace(10, "Ten");
  map.emplace(15, "Fifteen");
  map.emplace(5, "Five");

  Fmt::cout << "Hello, this is an example!" FMT_ENDL;

  FmtPrintf("%-7s", "array:");
  Fmt::cout << array << Fmt::endl;
  FmtPrintf("%-*s", 7, "map:");
  Fmt::cout << map << Fmt::endl;
  FmtPrintf("%*s", -7, "tuple:");
  Fmt::cout << std::make_tuple("number + char", 123456789, 'x') << Fmt::endl;
  FmtPrintf("%*s", -7, "pair:");
  Fmt::cout << std::make_pair("array", std::ref(nums)) << Fmt::endl;

  PRINTF("printf should also work just %s :%c %" PRIu8 " %p" FMT_ENDL, "fine", ')', num2, nullptr);

  ASSERT(1 != 1);
  LOG("This is never printed" FMT_ENDL);
}