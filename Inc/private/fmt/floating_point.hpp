#pragma once

#include <limits>

#include "fmt/fmt.h"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

template <size_t>
struct Ieee754Layout;

template <>
struct Ieee754Layout<4U>
{
  using uint_t  = uint32_t;

  enum : unsigned int
  {
    MANTISSA_WIDTH        = 23U,
    BIASED_EXPONENT_WIDTH = 8U,
    SIGN_WIDTH            = 1U
  };
};

template <>
struct Ieee754Layout<8U>
{
  using uint_t  = uint64_t;

  enum : unsigned int
  {
    MANTISSA_WIDTH        = 52U,
    BIASED_EXPONENT_WIDTH = 11U,
    SIGN_WIDTH            = 1U
  };
};

template <typename _float_t>
union FloatingPoint
{
  using float_t  = _float_t;
  using layout_t = std::conditional_t<std::numeric_limits<_float_t>::is_iec559,
    Ieee754Layout<sizeof(_float_t)>,
    void>;

  using uint_t   = typename layout_t::uint_t;
  using int_t    = std::make_signed_t<uint_t>;

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
