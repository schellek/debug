#pragma once

#include "fmt/ostream.hpp"
#include "fmt/stdout.h"


namespace fmt
{

template <typename T>
ostream & ostream::operator<<(T *value) noexcept
{
  *this << reinterpret_cast<const void *>(value);
  return *this;
}

} // namespace fmt
