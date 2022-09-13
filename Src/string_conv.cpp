#include <cstdint>

#include <limits>
#include <type_traits>

#include "fmt/string_conv.hpp"

using namespace std::string_view_literals;


namespace fmt
{

union ieee754_single
{
  using float_t = float;
  using uint_t  = uint32_t;
  using int_t   = int32_t;

  enum
  {
    MANTISSA_WIDTH        = 23U,
    BIASED_EXPONENT_WIDTH = 8U,
    SIGN_WIDTH            = 1U
  };

  static_assert(std::numeric_limits<float_t>::is_iec559);

  float_t f;
  uint_t w;
  struct
  {
    uint_t mantissa       : MANTISSA_WIDTH;
    uint_t biasedExponent : BIASED_EXPONENT_WIDTH;
    uint_t sign           : SIGN_WIDTH;
  } b;
};

union ieee754_double
{
  using float_t = double;
  using uint_t  = uint64_t;
  using int_t   = int64_t;

  enum
  {
    MANTISSA_WIDTH        = 52U,
    BIASED_EXPONENT_WIDTH = 11U,
    SIGN_WIDTH            = 1U
  };

  static_assert(std::numeric_limits<float_t>::is_iec559);

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
inline constexpr bool is_ieee754_v = std::is_same_v<T, ieee754_single> || std::is_same_v<T, ieee754_double>;

template <typename T>
using int_fast_t = std::conditional_t<sizeof(T) <= sizeof(int),
  std::conditional_t<std::is_unsigned_v<T>,
    unsigned int,
    int>,
  T>;

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool> = true>
std::string_view _toString(int_t value) noexcept;

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool> = true>
std::string_view _toString(ieee754_t value) noexcept;

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool> = true>
std::string_view _toHexString(int_t value, bool uppercase, bool prefix) noexcept;

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool> = true>
std::string_view _toOctString(int_t value, bool prefix) noexcept;

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool> = true>
static constexpr typename ieee754_t::uint_t biasedExponentMax(void) noexcept;

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool> = true>
static constexpr typename ieee754_t::uint_t bias(void) noexcept;

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool> = true>
static constexpr typename ieee754_t::int_t exponent(ieee754_t value) noexcept;

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool> = true>
static constexpr bool isNan(ieee754_t value) noexcept;

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool> = true>
static constexpr bool isInf(ieee754_t value) noexcept;

static char g_returnBuffer[25U];

std::string_view toString(bool value) noexcept
{
  return (value) ? "true"sv : "false"sv;
}

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool>>
std::string_view toString(int_t value) noexcept
{
  return _toString(static_cast<int_fast_t<int_t>>(value));
}

template <typename float_t, std::enable_if_t<std::is_floating_point_v<float_t>, bool>>
std::string_view toString(float_t value) noexcept
{
  using ieee754_t = std::conditional_t<std::is_same_v<float_t, float>, ieee754_single, ieee754_double>;

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

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool>>
std::string_view toHexString(int_t value, bool uppercase, bool prefix) noexcept
{
  return _toHexString(static_cast<int_fast_t<int_t>>(value), uppercase, prefix);
}

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool>>
std::string_view toOctString(int_t value, bool prefix) noexcept
{
  return _toOctString(static_cast<int_fast_t<int_t>>(value), prefix);
}

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool>>
std::string_view _toString(int_t value) noexcept
{
  int_t nextIterValue;
  bool sign = false;

  char *const end = g_returnBuffer + std::numeric_limits<int_t>::digits10 + ((std::is_signed_v<int_t>) ? 2U : 1U);
  char *begin = end;

  if constexpr (std::is_unsigned_v<int_t>)
  {
    /* Do nothing */
  }
  else if (value < 0)
  {
    sign = true;
    value = -value;
  }

  do
  {
    nextIterValue = value / 10;
    *(--begin) = static_cast<char>(value - (nextIterValue * 10)) + '0';
    value = nextIterValue;
  } while (value);

  if constexpr (std::is_unsigned_v<int_t>)
    /* Do nothing */;
  else if (sign)
    *(--begin) = '-';

  return {begin, static_cast<std::string_view::size_type>(end - begin)};
}


template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool>>
std::string_view _toString(ieee754_t value) noexcept
// using ieee754_t = ieee754_single;
// std::string_view _toString(ieee754_t value)
{
  using float_t = typename ieee754_t::float_t;
  using uint_t  = typename ieee754_t::uint_t;
  using int_t   = typename ieee754_t::int_t;

  const int_t e = exponent(value);
  // const uint_t f = (value.b.mantissa >> (ieee754_t::MANTISSA_WIDTH - e)) | uint_t{1} << e;

  // float_t vMinus, vPlus;
  // uint_t mMinus, mPlus;

  // if (e < 0)
  // {
  //   mMinus = 1U << e;
  // }
  // else
  // {
  //   mMinus = 1U;
  // }

  return "<not implemented>"sv;
}

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool>>
std::string_view _toHexString(int_t value, bool uppercaseLetters, bool prefix) noexcept
{
  static constexpr char HEX_LOOKUP[2U][16U] =
  {
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' },
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' }
  };

  const char (&hexLookup)[16U] = HEX_LOOKUP[uppercaseLetters ? 1U : 0U];

  char *const end = g_returnBuffer + ((sizeof(int_t) * 8U) / 4U) + 2U;
  char *begin = end;

  do
  {
    *(--begin) = hexLookup[value & 0xFU];
    value >>= 4U;
  } while (value);

  if (prefix)
  {
    *(--begin) = uppercaseLetters ? 'X' : 'x';
    *(--begin) = '0';
  }

  return {begin, static_cast<std::string_view::size_type>(end - begin)};
}

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool>>
std::string_view _toOctString(int_t value, bool prefix) noexcept
{
  char *const end = g_returnBuffer + ((sizeof(int_t) * 8U) / 3U) + 2U;
  char *begin = end;

  do
  {
    *(--begin) = (value & 0x7) + '0';
    value >>= 3U;
  } while (value);

  if (prefix && (*begin != '0'))
    *(--begin) = '0';

  return {begin, static_cast<std::string_view::size_type>(end - begin)};
}

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool>>
static constexpr typename ieee754_t::uint_t biasedExponentMax(void) noexcept
{
  return static_cast<typename ieee754_t::uint_t>((1U << ieee754_t::BIASED_EXPONENT_WIDTH) - 1U);
}

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool>>
static constexpr typename ieee754_t::uint_t bias(void) noexcept
{
  return static_cast<typename ieee754_t::uint_t>((1U << (ieee754_t::BIASED_EXPONENT_WIDTH - 1U)) - 1U);
}

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool>>
static constexpr typename ieee754_t::int_t exponent(ieee754_t value) noexcept
{
  return static_cast<typename ieee754_t::int_t>(value.b.biasedExponent - bias<ieee754_t>());
}

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool>>
static constexpr bool isNan(ieee754_t value) noexcept
{
  return (value.b.biasedExponent == biasedExponentMax<ieee754_t>()) && (value.b.mantissa != 0);
}

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool>>
static constexpr bool isInf(ieee754_t value) noexcept
{
  return (value.b.biasedExponent == biasedExponentMax<ieee754_t>()) && (value.b.mantissa == 0);
}

#define INSTANCIATE_TEMPLATE(TYPE) template std::string_view toString(TYPE) noexcept
#define INSTANCIATE_INTEGRAL
#define INSTANCIATE_FLOATING_POINT
#include "instanciate_template.hpp"
#undef INSTANCIATE_TEMPLATE

#define INSTANCIATE_TEMPLATE(TYPE) template std::string_view toHexString(TYPE, bool, bool) noexcept
#define INSTANCIATE_INTEGRAL
#include "instanciate_template.hpp"
#undef INSTANCIATE_TEMPLATE

#define INSTANCIATE_TEMPLATE(TYPE) template std::string_view toOctString(TYPE, bool) noexcept
#define INSTANCIATE_INTEGRAL
#include "instanciate_template.hpp"
#undef INSTANCIATE_TEMPLATE

} // namespace fmt
