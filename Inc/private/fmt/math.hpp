#ifndef FMT_MATH_HPP
#define FMT_MATH_HPP

#include "fmt/fmt.h"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

template <typename T, EnableIfT<IsIntegralV<T>> = true>
constexpr unsigned int Log2(T value)
{
  constexpr unsigned int width = sizeof(T) * 8u;
  unsigned int log2 = 0u, i = 0u;

  for (; i < width; ++i)
  {
    if (value == (T{1} << i))
    {
      log2 = i;
      break;
    }
  }

  return log2;
}

constexpr int32_t Pow(int32_t x, uint8_t y)
{
  int32_t retval = 1;

  for (; y > 0u; --y)
    retval *= x;

  return retval;
}

FMT_END_NAMESPACE

#endif /* FMT_MATH_HPP */