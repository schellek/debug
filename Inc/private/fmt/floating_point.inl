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
constexpr typename FloatingPoint<T>::UIntT FloatingPoint<T>::bias(void) noexcept
{
  return static_cast<UIntT>((1U << (LayoutT::BIASED_EXPONENT_WIDTH - 1U)) - 1U);
}

template <typename T>
constexpr typename FloatingPoint<T>::UIntT FloatingPoint<T>::biasedExponentMax(void) noexcept
{
  return static_cast<UIntT>((1U << LayoutT::BIASED_EXPONENT_WIDTH) - 1U);
}

template <typename T>
constexpr typename FloatingPoint<T>::IntT FloatingPoint<T>::exponent(void) noexcept
{
  return static_cast<IntT>(b.biasedExponent - IntegralConstant<UIntT, bias()>::Value);
}

template <typename T>
constexpr bool FloatingPoint<T>::isNan(void) noexcept
{
  return (b.biasedExponent == IntegralConstant<UIntT, biasedExponentMax()>::Value) && (b.mantissa != 0);
}

template <typename T>
constexpr bool FloatingPoint<T>::isInf(void) noexcept
{
  return (b.biasedExponent == IntegralConstant<UIntT, biasedExponentMax()>::Value) && (b.mantissa == 0);
}

template <typename T>
constexpr bool FloatingPoint<T>::isZero(void) noexcept
{
  return (b.biasedExponent == 0) && (b.mantissa == 0);
}

FMT_END_NAMESPACE
