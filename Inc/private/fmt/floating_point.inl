#pragma once

#include "fmt/floating_point.hpp"

FMT_BEGIN_NAMESPACE

template <typename T>
constexpr FloatingPoint<T>::FloatingPoint(FloatT f) noexcept
  : f{f}
{
}

template <typename T>
constexpr FloatingPoint<T>::FloatingPoint(UIntT w) noexcept
  : w{w}
{
}

template <typename T>
inline typename FloatingPoint<T>::IntT FloatingPoint<T>::exponent(void) noexcept
{
  return static_cast<IntT>(b.biasedExponent - bias);
}

template <typename T>
inline bool FloatingPoint<T>::isNan(void) noexcept
{
  return LayoutT::hasNan() ? ((b.biasedExponent == biasedExponentMax) && (b.mantissa > 0U)) : false;
}

template <typename T>
inline bool FloatingPoint<T>::isInf(void) noexcept
{
  return LayoutT::hasInf() ? ((b.biasedExponent == biasedExponentMax) && (b.mantissa == 0U)) : false;
}

template <typename T>
inline bool FloatingPoint<T>::isZero(void) noexcept
{
  return (b.biasedExponent == 0U) && (b.mantissa == 0U);
}

FMT_END_NAMESPACE
