#pragma once

#include <limits>

#include "fmt/fmt.h"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

enum FloatingPointStandard
{
  Unknown = -1,
  Ieee754,
  Iec559 = Ieee754,
  Bfloat
};

template <FloatingPointStandard, size_t>
struct FloatingPointLayout;

template <>
struct FloatingPointLayout<Ieee754, 2U>
{
  enum : unsigned int
  {
    MANTISSA_WIDTH        = 10U,
    BIASED_EXPONENT_WIDTH = 5U,
    SIGN_WIDTH            = 1U
  };
};

template <>
struct FloatingPointLayout<Bfloat, 2U>
{
  enum : unsigned int
  {
    MANTISSA_WIDTH        = 7U,
    BIASED_EXPONENT_WIDTH = 8U,
    SIGN_WIDTH            = 1U
  };
};

template <>
struct FloatingPointLayout<Ieee754, 4U>
{
  enum : unsigned int
  {
    MANTISSA_WIDTH        = 23U,
    BIASED_EXPONENT_WIDTH = 8U,
    SIGN_WIDTH            = 1U
  };
};

template <>
struct FloatingPointLayout<Ieee754, 8U>
{
  enum : unsigned int
  {
    MANTISSA_WIDTH        = 52U,
    BIASED_EXPONENT_WIDTH = 11U,
    SIGN_WIDTH            = 1U
  };
};

template <>
struct FloatingPointLayout<Ieee754, 16U>
{
  enum : unsigned int
  {
    MANTISSA_WIDTH        = 112U,
    BIASED_EXPONENT_WIDTH = 15U,
    SIGN_WIDTH            = 1U
  };
};

template <typename T>
union FloatingPoint
{
private:
  static constexpr FloatingPointStandard _standard(void) noexcept;

public:
  using float_t  = T;
  using layout_t = FloatingPointLayout<_standard(), sizeof(float_t)>;
  using uint_t   = UnsignedIntT<sizeof(float_t)>;
  using int_t    = SignedIntT<sizeof(float_t)>;

  enum : unsigned int
  {
    MANTISSA_WIDTH        = layout_t::MANTISSA_WIDTH,
    BIASED_EXPONENT_WIDTH = layout_t::BIASED_EXPONENT_WIDTH,
    SIGN_WIDTH            = layout_t::SIGN_WIDTH
  };

  float_t f;
  uint_t w;
  struct
  {
    uint_t mantissa       : MANTISSA_WIDTH;
    uint_t biasedExponent : BIASED_EXPONENT_WIDTH;
    uint_t sign           : SIGN_WIDTH;
  } b;

private:
  static constexpr uint_t _bias(void) noexcept;
  static constexpr uint_t _biasedExponentMax(void) noexcept;
  static constexpr uint_t biasedExponentMax = _biasedExponentMax();

public:
  constexpr FloatingPoint(float_t f) noexcept;
  constexpr FloatingPoint(uint_t w) noexcept;

  static constexpr uint_t bias = _bias();

  constexpr int_t exponent(void) noexcept;

  constexpr bool isZero(void) noexcept;
  constexpr bool isInf(void) noexcept;
  constexpr bool isNan(void) noexcept;
};

FMT_END_NAMESPACE

#include "fmt/floating_point.inl"
