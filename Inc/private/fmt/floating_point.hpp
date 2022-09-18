#pragma once

#include <climits>
#include <cfloat>
#include "fmt/fmt.h"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

template <unsigned int M = 0U, unsigned int B = 0U>
struct __FloatingPointLayout
{
  enum : unsigned int
  {
    MANTISSA_WIDTH        = M,
    BIASED_EXPONENT_WIDTH = B,

    REQUIRED_WIDTH        = M + B + 1
  };

  static constexpr bool isValid(void) noexcept { return (M > 0U) && (B > 0U); }
};

template <typename T>
struct FloatingPointLayout : __FloatingPointLayout<> {};

#ifdef FMT_FLOAT16_SUPPORT
template <>
struct FloatingPointLayout<FmtFloat16> : __FloatingPointLayout<10U, 5U> {};
#endif // FMT_FLOAT16_SUPPORT

#ifdef FMT_BFLOAT16_SUPPORT
template <>
struct FloatingPointLayout<FmtBFloat16> : __FloatingPointLayout<7U, 8U> {};
#endif // FMT_BFLOAT16_SUPPORT

template <>
struct FloatingPointLayout<float> : __FloatingPointLayout<FLT_MANT_DIG, 1> {};

template <>
struct FloatingPointLayout<double> : __FloatingPointLayout<DBL_MANT_DIG, 1> {};

template <>
struct FloatingPointLayout<long double> : __FloatingPointLayout<LDBL_MANT_DIG, 1> {};

#ifdef FMT_FLOAT80_SUPPORT
template <>
struct FloatingPointLayout<FmtFloat80> : __FloatingPointLayout<64U, 15U> {};
#endif // FMT_FLOAT80_SUPPORT

#ifdef FMT_FLOAT128_SUPPORT
template <>
struct FloatingPointLayout<FmtFloat128> : __FloatingPointLayout<112U, 15U> {};
#endif // FMT_FLOAT128_SUPPORT

template <typename T, typename LayoutT, unsigned int P>
struct __FloatingPointBitfield
{
  T mantissa       : LayoutT::MANTISSA_WIDTH;
  T biasedExponent : LayoutT::BIASED_EXPONENT_WIDTH;
  T sign           : 1U;
  T padding        : P;
};

template <typename T, typename LayoutT>
struct __FloatingPointBitfield<T, LayoutT, 0U>
{
  T mantissa       : LayoutT::MANTISSA_WIDTH;
  T biasedExponent : LayoutT::BIASED_EXPONENT_WIDTH;
  T sign           : 1U;
};

template <typename T, typename LayoutT>
using FloatingPointBitfield = __FloatingPointBitfield<T, LayoutT, sizeof(T) * 8U - LayoutT::REQUIRED_WIDTH>;

template <typename T>
union FloatingPoint
{
public:
  using FloatT  = T;
  using UIntT   = UnsignedIntT<sizeof(FloatT)>;
  using IntT    = SignedIntT<sizeof(FloatT)>;

private:
  using LayoutT = FloatingPointLayout<T>;
  static_assert(LayoutT::isValid());

public:
  FloatT f;
  alignas(alignof(T)) UIntT w;
  alignas(alignof(T)) FloatingPointBitfield<UIntT, LayoutT> b;

private:
  static constexpr UIntT bias(void) noexcept;
  static constexpr UIntT biasedExponentMax(void) noexcept;

public:
  constexpr FloatingPoint(FloatT f) noexcept;
  constexpr FloatingPoint(UIntT w) noexcept;

  constexpr IntT exponent(void) noexcept;

  constexpr bool isZero(void) noexcept;
  constexpr bool isInf(void) noexcept;
  constexpr bool isNan(void) noexcept;
};

FMT_END_NAMESPACE

#include "fmt/floating_point.inl"
