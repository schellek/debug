#pragma once

#include "fmt/ostream.hpp"
#include "fmt/string_conv.hpp"


namespace fmt
{

ostream& ostream::operator<<(integral auto value)
{
  write(int2str(value));
  return *this;
}

ostream& ostream::operator<<(floating_point auto value)
{
  write(float2string(value));
  return *this;
}

} // namespace fmt
