#pragma once

#include "fmt/floating_point.hpp"

FMT_BEGIN_NAMESPACE

template <typename T>
constexpr FloatingPointStandard FloatingPoint<T>::_standard(void) noexcept
{
  FloatingPointStandard standard = Unknown;

  if constexpr (std::numeric_limits<T>::is_iec559)
    standard = Iec559;
#ifdef FMT_FLOAT128_SUPPORT
  else if constexpr (IsSame<T, fmt_float128_t>::value)
    standard = Iec559;
#endif // FMT_FLOAT128_SUPPORT
#ifdef FMT_FLOAT16_SUPPORT
  else if constexpr (IsSame<T, fmt_float16_t>::value)
    standard = Iec559;
#endif // FMT_FLOAT16_SUPPORT
#ifdef FMT_BFLOAT16_SUPPORT
  else if constexpr (IsSame<T, fmt_bfloat16_t>::value)
    standard = Bfloat;
#endif // FMT_BFLOAT16_SUPPORT

  return standard;
}

template <typename T>
constexpr FloatingPoint<T>::FloatingPoint(float_t f) noexcept
  : f{f}
{
}

template <typename T>
constexpr FloatingPoint<T>::FloatingPoint(uint_t w) noexcept
  : w{w}
{
}

template <typename T>
constexpr typename FloatingPoint<T>::uint_t FloatingPoint<T>::_bias(void) noexcept
{
  return static_cast<uint_t>((1U << (BIASED_EXPONENT_WIDTH - 1U)) - 1U);
}

template <typename T>
constexpr typename FloatingPoint<T>::uint_t FloatingPoint<T>::_biasedExponentMax(void) noexcept
{
  return static_cast<uint_t>((1U << BIASED_EXPONENT_WIDTH) - 1U);
}

template <typename T>
constexpr typename FloatingPoint<T>::int_t FloatingPoint<T>::exponent(void) noexcept
{
  return static_cast<int_t>(b.biasedExponent - bias);
}

template <typename T>
constexpr bool FloatingPoint<T>::isNan(void) noexcept
{
  return (b.biasedExponent == biasedExponentMax) && (b.mantissa != 0);
}

template <typename T>
constexpr bool FloatingPoint<T>::isInf(void) noexcept
{
  return (b.biasedExponent == biasedExponentMax) && (b.mantissa == 0);
}

template <typename T>
constexpr bool FloatingPoint<T>::isZero(void) noexcept
{
  return (b.biasedExponent == 0) && (b.mantissa == 0);
}

FMT_END_NAMESPACE
