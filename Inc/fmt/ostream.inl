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

template <typename T, size_t N>
ostream & ostream::operator<<(const std::array<T, N> &array) noexcept
{
  using array_t = std::array<T, N>;

  *this << '[';

  for (typename array_t::const_iterator begin = array.cbegin(), end = array.cend(), it = begin; it != end; ++it)
  {
    if (it != begin)
      *this << ", ";

    *this << *it;
  }

  *this << ']';

  return *this;
}

template <size_t idx, typename ...T>
ostream & ostream::operator<<(const std::tuple<T...> &tuple) noexcept
{
  if constexpr (idx < sizeof...(T))
  {
    if constexpr (idx == 0U)
      *this << '(';
    else
      *this << ", ";

    *this << std::get<idx>(tuple);
    this->operator<<<idx + 1, T...>(tuple);
  }
  else
  {
    *this << ')';
  }

  return *this;
}

template <typename T1, typename T2>
ostream & ostream::operator<<(const std::pair<T1, T2> &pair) noexcept
{
  print(*this, '(', std::get<0>(pair), ", ", std::get<1>(pair), ')');
  return *this;
}

inline ostream & endl(ostream &stream) noexcept
{
  using namespace std::string_view_literals;

  stream.write(FMT_ENDL ""sv);
  return stream;
}

template <typename ...T>
void print(ostream &stream, T && ...value) noexcept
{
  ((stream << value), ...);
}

} // namespace fmt
