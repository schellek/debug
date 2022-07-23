#pragma once

#include "fmt/ostream.hpp"
#include "fmt/stdout.h"


namespace fmt
{

template <typename _Ty>
ostream & ostream::operator<<(_Ty *value) noexcept
{
  *this << reinterpret_cast<const void *>(value);
  return *this;
}

template <typename _Ty, size_t N>
ostream & ostream::operator<<(const std::array<_Ty, N> &array) noexcept
{
  using array_t = std::array<_Ty, N>;

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

template <size_t idx, typename ..._Ty>
ostream & ostream::operator<<(const std::tuple<_Ty...> &tuple) noexcept
{
  if constexpr (idx < sizeof...(_Ty))
  {
    if constexpr (idx == 0U)
      *this << '(';
    else
      *this << ", ";

    *this << std::get<idx>(tuple);
    this->operator<<<idx + 1, _Ty...>(tuple);
  }
  else
  {
    *this << ')';
  }

  return *this;
}

template <typename _Ty1, typename _Ty2>
ostream & ostream::operator<<(const std::pair<_Ty1, _Ty2> &pair) noexcept
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

template <typename ..._Ty>
void print(ostream &stream, _Ty && ...value) noexcept
{
  ((stream << value), ...);
}

} // namespace fmt
