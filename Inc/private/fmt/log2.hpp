#pragma once

#include "fmt/fmt.h"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

template <typename T, EnableIfT<IsIntegralV<T>> = true>
constexpr unsigned int Log2(T value) noexcept
{
  constexpr unsigned int width = sizeof(T) * 8U;
  unsigned int log2 = 0U, i = 0U;

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

FMT_END_NAMESPACE
