#pragma once

#include "fmt/ostream.hpp"

FMT_BEGIN_NAMESPACE

template <typename T, EnableIfT<IsPointerV<T>>>
ostream & ostream::operator<<(T value) noexcept
{
  *this << reinterpret_cast<const void *>(value);
  return *this;
}

template <typename T, EnableIfT<IsSmartPointerV<T>>>
ostream & ostream::operator<<(T &value) noexcept
{
  *this << value.get();
  return *this;
}

FMT_END_NAMESPACE
