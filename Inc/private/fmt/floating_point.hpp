#ifndef FMT_FLOATING_POINT_HPP
#define FMT_FLOATING_POINT_HPP

#include "fmt/fmt.h"
#include "fmt/type_traits.hpp"
#include "fmt/extended_types.h"
#include "fmt/math.hpp"

FMT_BEGIN_NAMESPACE

template <unsigned int MantW = 0u, unsigned int BiasedExpW = 0u, bool InfNan = true>
struct __FloatingPointLayout
{
  enum : unsigned int
  {
    BASE                  = 2u,
    MANTISSA_WIDTH        = MantW,
    BIASED_EXPONENT_WIDTH = BiasedExpW,
    REQUIRED_WIDTH        = MANTISSA_WIDTH + BIASED_EXPONENT_WIDTH + 1
  };

  static constexpr bool isValid(void) { return (MANTISSA_WIDTH > 0u) && (BIASED_EXPONENT_WIDTH > 0u); }
  static constexpr bool hasInf(void)  { return InfNan; }
  static constexpr bool hasNan(void)  { return InfNan; }
};

template <unsigned int MantW = 0u, unsigned int BiasedExpW = 0u>
struct _FloatingPointLayout : __FloatingPointLayout<MantW, BiasedExpW> {};

template <>
struct _FloatingPointLayout<7u, 8u> : __FloatingPointLayout<7u, 8u, false> {};

using BrainFloat       = _FloatingPointLayout<7u,   8u>;
using Ieee754Half      = _FloatingPointLayout<10u,  5u>;
using Ieee754Single    = _FloatingPointLayout<23u,  8u>;
using Ieee754SingleExt = _FloatingPointLayout<31u,  8u>;
using Ieee754Double    = _FloatingPointLayout<52u,  11u>;
using Ieee754DoubleExt = _FloatingPointLayout<64u,  15u>;
using Ieee754Quadruple = _FloatingPointLayout<112u, 15u>;

template <unsigned int MantDig, unsigned int MaxExp>
using FloatHLayout = _FloatingPointLayout<MantDig - 1u, Log2(MaxExp) + 1u>;

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

template <typename T, typename tLayout, unsigned int P>
struct __FloatingPointBitfield
{
  T mantissa       : tLayout::MANTISSA_WIDTH;
  T biasedExponent : tLayout::BIASED_EXPONENT_WIDTH;
  T sign           : 1u;
  T padding        : P;
};

template <typename T, typename tLayout>
struct __FloatingPointBitfield<T, tLayout, 0u>
{
  T mantissa       : tLayout::MANTISSA_WIDTH;
  T biasedExponent : tLayout::BIASED_EXPONENT_WIDTH;
  T sign           : 1u;
};

template <typename T, typename tLayout>
using FloatingPointBitfield = __FloatingPointBitfield<T, tLayout, sizeof(T) * 8u - tLayout::REQUIRED_WIDTH>;

template <typename T>
union FloatingPoint
{
public:
  using tFloat  = T;
  using tUInt   = UnsignedIntT<sizeof(tFloat)>;
  using tInt    = SignedIntT<sizeof(tFloat)>;
  using tLayout = FloatingPointLayout<T>;
  static_assert(tLayout::isValid());

  tFloat f;
  alignas(alignof(T)) tUInt w;
  alignas(alignof(T)) FloatingPointBitfield<tUInt, tLayout> b;

  constexpr FloatingPoint(tFloat f);
  constexpr FloatingPoint(tUInt w);

  tInt exponent(void);

  bool isZero(void);
  bool isInf(void);
  bool isNan(void);

private:
  static constexpr tUInt bias              = (1u << (tLayout::BIASED_EXPONENT_WIDTH - 1u)) - 1u;
  static constexpr tUInt biasedExponentMax = (1u << tLayout::BIASED_EXPONENT_WIDTH) - 1u;
};

FMT_END_NAMESPACE

#include "fmt/floating_point.inl"

#endif /* FMT_FLOATING_POINT_HPP */
