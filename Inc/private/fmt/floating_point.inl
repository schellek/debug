#ifndef FMT_FLOATING_POINT_INL
#define FMT_FLOATING_POINT_INL

#include "fmt/floating_point.hpp"

FMT_BEGIN_NAMESPACE

template <typename T>
constexpr FloatingPoint<T>::FloatingPoint(tFloat f)
  : f{f}
{
}

template <typename T>
constexpr FloatingPoint<T>::FloatingPoint(tUInt w)
  : w{w}
{
}

template <typename T>
inline typename FloatingPoint<T>::tInt FloatingPoint<T>::exponent(void)
{
  return static_cast<tInt>(b.biasedExponent - bias);
}

template <typename T>
inline bool FloatingPoint<T>::isNan(void)
{
  return tLayout::hasNan() ? ((b.biasedExponent == biasedExponentMax) && (b.mantissa > 0u)) : false;
}

template <typename T>
inline bool FloatingPoint<T>::isInf(void)
{
  return tLayout::hasInf() ? ((b.biasedExponent == biasedExponentMax) && (b.mantissa == 0u)) : false;
}

template <typename T>
inline bool FloatingPoint<T>::isZero(void)
{
  return (b.biasedExponent == 0u) && (b.mantissa == 0u);
}

FMT_END_NAMESPACE

#endif /* FMT_FLOATING_POINT_INL */
