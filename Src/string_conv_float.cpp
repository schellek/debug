#include <limits>

#include "fmt/string_conv.hpp"
#include "fmt/string_conv_buffer.hpp"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

using namespace std::string_view_literals;

union Ieee754Single
{
  using float_t = float;
  using uint_t  = uint32_t;
  using int_t   = int32_t;

  static_assert(std::numeric_limits<float_t>::is_iec559);

  enum : unsigned int
  {
    MANTISSA_WIDTH        = 23U,
    BIASED_EXPONENT_WIDTH = 8U,
    SIGN_WIDTH            = 1U
  };

  float_t f;
  uint_t w;
  struct
  {
    uint_t mantissa       : MANTISSA_WIDTH;
    uint_t biasedExponent : BIASED_EXPONENT_WIDTH;
    uint_t sign           : SIGN_WIDTH;
  } b;
};

union Ieee754Double
{
  using float_t = double;
  using uint_t  = uint64_t;
  using int_t   = int64_t;

  static_assert(std::numeric_limits<float_t>::is_iec559);

  enum : unsigned int
  {
    MANTISSA_WIDTH        = 52U,
    BIASED_EXPONENT_WIDTH = 11U,
    SIGN_WIDTH            = 1U
  };

  float_t f;
  uint_t w;
  struct
  {
    uint_t mantissa       : MANTISSA_WIDTH;
    uint_t biasedExponent : BIASED_EXPONENT_WIDTH;
    uint_t sign           : SIGN_WIDTH;
  } b;
};

template <typename T>
using IsIeee754 = IsAnyOf<T, Ieee754Single, Ieee754Double>;

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool> = true>
static std::string_view _toString(ieee754_t value) noexcept;

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool> = true>
static constexpr typename ieee754_t::uint_t biasedExponentMax(void) noexcept;

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool> = true>
static constexpr typename ieee754_t::uint_t bias(void) noexcept;

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool> = true>
static constexpr typename ieee754_t::int_t exponent(ieee754_t value) noexcept;

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool> = true>
static constexpr bool isNan(ieee754_t value) noexcept;

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool> = true>
static constexpr bool isInf(ieee754_t value) noexcept;

template <typename float_t, std::enable_if_t<std::is_floating_point<float_t>::value, bool>>
std::string_view toString(float_t value) noexcept
{
  using ieee754_t = std::conditional_t<std::is_same<float_t, float>::value, Ieee754Single, Ieee754Double>;

  std::string_view retval;
  ieee754_t num{value};

  if (num.w == 0U)
    retval = "0"sv;

  else if (isInf(num))
    retval = (num.b.sign == 0U) ? "inf"sv : "-inf"sv;

  else if (isNan(num))
    retval = (num.b.sign == 0U) ? "nan"sv : "-nan"sv;

  else
    retval = _toString(num);

  return retval;
}

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool>>
static std::string_view _toString(ieee754_t value) noexcept
{
  using float_t = typename ieee754_t::float_t;
  using uint_t  = typename ieee754_t::uint_t;
  using int_t   = typename ieee754_t::int_t;

  const int_t e = exponent(value);
  static_cast<void>(e);

  return "<float>"sv;
}

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool>>
static constexpr typename ieee754_t::uint_t biasedExponentMax(void) noexcept
{
  return static_cast<typename ieee754_t::uint_t>((1U << ieee754_t::BIASED_EXPONENT_WIDTH) - 1U);
}

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool>>
static constexpr typename ieee754_t::uint_t bias(void) noexcept
{
  return static_cast<typename ieee754_t::uint_t>((1U << (ieee754_t::BIASED_EXPONENT_WIDTH - 1U)) - 1U);
}

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool>>
static constexpr typename ieee754_t::int_t exponent(ieee754_t value) noexcept
{
  return static_cast<typename ieee754_t::int_t>(value.b.biasedExponent - bias<ieee754_t>());
}

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool>>
static constexpr bool isNan(ieee754_t value) noexcept
{
  return (value.b.biasedExponent == biasedExponentMax<ieee754_t>()) && (value.b.mantissa != 0);
}

template <typename ieee754_t, std::enable_if_t<IsIeee754<ieee754_t>::value, bool>>
static constexpr bool isInf(ieee754_t value) noexcept
{
  return (value.b.biasedExponent == biasedExponentMax<ieee754_t>()) && (value.b.mantissa == 0);
}

FMT_END_NAMESPACE

#define INSTANCIATE_TEMPLATE(TYPE) template std::string_view FMT_ABI::toString(TYPE) noexcept
#define INSTANCIATE_FLOATING_POINT
#include "fmt/instanciate_template.hpp"
#undef INSTANCIATE_TEMPLATE
