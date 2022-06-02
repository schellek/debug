#pragma once

#include <string_view>

#include "fmt/concepts.hpp"


namespace fmt
{

std::string_view int2str(integral auto value);
std::string_view int2hexstr(integral auto value, bool uppercaseLetters);
std::string_view int2octstr(integral auto value);
std::string_view float2string(floating_point auto value);

} // namespace fmt

#include "string_conv.inl"
