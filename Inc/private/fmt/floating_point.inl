#pragma once

#include "fmt/floating_point.hpp"

FMT_BEGIN_NAMESPACE


template <typename _float_t>
constexpr FloatingPoint<_float_t>::FloatingPoint(float_t f) noexcept
  : f{f}
{
}

template <typename _float_t>
constexpr FloatingPoint<_float_t>::FloatingPoint(uint_t w) noexcept
  : w{w}
{
}

template <typename _float_t>
constexpr typename FloatingPoint<_float_t>::uint_t FloatingPoint<_float_t>::_bias(void) noexcept
{
  return static_cast<uint_t>((1U << (BIASED_EXPONENT_WIDTH - 1U)) - 1U);
}

template <typename _float_t>
constexpr typename FloatingPoint<_float_t>::uint_t FloatingPoint<_float_t>::_biasedExponentMax(void) noexcept
{
  return static_cast<uint_t>((1U << BIASED_EXPONENT_WIDTH) - 1U);
}

template <typename _float_t>
constexpr typename FloatingPoint<_float_t>::int_t FloatingPoint<_float_t>::exponent(void) noexcept
{
  return static_cast<int_t>(b.biasedExponent - bias);
}

template <typename _float_t>
constexpr bool FloatingPoint<_float_t>::isNan(void) noexcept
{
  return (b.biasedExponent == biasedExponentMax) && (b.mantissa != 0);
}

template <typename _float_t>
constexpr bool FloatingPoint<_float_t>::isInf(void) noexcept
{
  return (b.biasedExponent == biasedExponentMax) && (b.mantissa == 0);
}

template <typename _float_t>
constexpr bool FloatingPoint<_float_t>::isZero(void) noexcept
{
  return (b.biasedExponent == 0) && (b.mantissa == 0);
}

FMT_END_NAMESPACE
