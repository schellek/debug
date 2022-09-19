#pragma once

#include "fmt/fmt.h"
#include "fmt/type_traits.hpp"
#include "fmt/extended_types.h"
#include "fmt/log2.hpp"

FMT_BEGIN_NAMESPACE

template <unsigned int M = 0U, unsigned int B = 0U, bool InfNan = true>
struct __FloatingPointLayout
{
  enum : unsigned int
  {
    MANTISSA_WIDTH        = M,
    BIASED_EXPONENT_WIDTH = B,

    REQUIRED_WIDTH        = M + B + 1
  };

  static constexpr bool isValid(void) noexcept { return (M > 0U) && (B > 0U); }
  static constexpr bool hasInf(void) noexcept  { return InfNan; }
  static constexpr bool hasNan(void) noexcept  { return InfNan; }
};

template <unsigned int M = 0U, unsigned int B = 0U>
struct _FloatingPointLayout : __FloatingPointLayout<M, B> {};

template <>
struct _FloatingPointLayout<7U, 8U> : __FloatingPointLayout<7U, 8U, false> {};

using BrainFloat       = _FloatingPointLayout<7U,   8U>;
using Ieee754Half      = _FloatingPointLayout<10U,  5U>;
using Ieee754Single    = _FloatingPointLayout<23U,  8U>;
using Ieee754SingleExt = _FloatingPointLayout<31U,  8U>;
using Ieee754Double    = _FloatingPointLayout<52U,  11U>;
using Ieee754DoubleExt = _FloatingPointLayout<64U,  15U>;
using Ieee754Quadruple = _FloatingPointLayout<112U, 15U>;

template <unsigned int MantDig, unsigned int MaxExp>
using FloatHLayout = _FloatingPointLayout<MantDig - 1U, Log2(MaxExp) + 1U>;

template <typename T>
struct FloatingPointLayout : _FloatingPointLayout<> {};

#ifdef FMT_FLOAT16_SUPPORT
template <>
struct FloatingPointLayout<FmtFloat16> : FloatHLayout<FLT16_MANT_DIG, FLT16_MAX_EXP> {};
#endif // FMT_FLOAT16_SUPPORT

template <>
struct FloatingPointLayout<float> : FloatHLayout<FLT_MANT_DIG, FLT_MAX_EXP> {};

template <>
struct FloatingPointLayout<double> : FloatHLayout<DBL_MANT_DIG, DBL_MAX_EXP> {};

template <>
struct FloatingPointLayout<long double> : FloatHLayout<LDBL_MANT_DIG, LDBL_MAX_EXP> {};

#ifdef FMT_FLOAT80_SUPPORT
template <>
struct FloatingPointLayout<FmtFloat80> : Ieee754DoubleExt {};
#endif // FMT_FLOAT80_SUPPORT

#ifdef FMT_FLOAT128_SUPPORT
template <>
struct FloatingPointLayout<FmtFloat128> : Ieee754Quadruple {};
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

  static constexpr UIntT bias              = (1U << (LayoutT::BIASED_EXPONENT_WIDTH - 1U)) - 1U;
  static constexpr UIntT biasedExponentMax = (1U << LayoutT::BIASED_EXPONENT_WIDTH) - 1U;

public:
  FloatT f;
  alignas(alignof(T)) UIntT w;
  alignas(alignof(T)) FloatingPointBitfield<UIntT, LayoutT> b;

public:
  constexpr FloatingPoint(FloatT f) noexcept;
  constexpr FloatingPoint(UIntT w) noexcept;

  IntT exponent(void) noexcept;

  bool isZero(void) noexcept;
  bool isInf(void) noexcept;
  bool isNan(void) noexcept;
};

FMT_END_NAMESPACE

#include "fmt/floating_point.inl"
