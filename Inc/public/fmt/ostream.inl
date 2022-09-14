#pragma once

#include "fmt/ostream.hpp"

FMT_BEGIN_NAMESPACE

template <typename ptr_t, std::enable_if_t<std::is_pointer<ptr_t>::value, bool>>
ostream & ostream::operator<<(ptr_t value) noexcept
{
  *this << reinterpret_cast<const void *>(value);
  return *this;
}

template <typename ptr_t, std::enable_if_t<IsSmartPtr<ptr_t>::value, bool>>
ostream & ostream::operator<<(ptr_t &value) noexcept
{
  *this << value.get();
  return *this;
}

FMT_END_NAMESPACE
