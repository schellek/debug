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

  enum : uint_t
  {
    C_FRACTION_WIDTH = 23U,
    C_EXPONENT_WIDTH = 8U,
    C_SIGN_WIDTH     = 1U,

    C_BIAS           = 127U,
    C_INFINITY       = 0x7F800000U,
    C_NAN            = 0x7FC00000U
  };

  static_assert(std::numeric_limits<float_t>::is_iec559);

  float_t f;
  uint_t w;
  struct
  {
    uint_t fraction : C_FRACTION_WIDTH;
    uint_t exponent : C_EXPONENT_WIDTH;
    uint_t sign     : C_SIGN_WIDTH;
  } b;
};

union ieee754_double
{
  using float_t = double;
  using uint_t  = uint64_t;

  enum : uint_t
  {
    C_FRACTION_WIDTH = 52U,
    C_EXPONENT_WIDTH = 11U,
    C_SIGN_WIDTH     = 1U,

    C_BIAS           = 1023U,
    C_INFINITY       = 0x7FF0000000000000U,
    C_NAN            = 0x7FF8000000000000U
  };

  static_assert(std::numeric_limits<float_t>::is_iec559);

  float_t f;
  uint_t w;
  struct
  {
    uint_t fraction : C_FRACTION_WIDTH;
    uint_t exponent : C_EXPONENT_WIDTH;
    uint_t sign     : C_SIGN_WIDTH;
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

constexpr uint16_t g_returnBufferLen = 25U;
static char g_returnBuffer[g_returnBufferLen];

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool> = true>
std::string_view _toString(int_t value) noexcept;

template <typename ieee754_t, std::enable_if_t<is_ieee754_v<ieee754_t>, bool> = true>
std::string_view _toString(ieee754_t value) noexcept;

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool> = true>
std::string_view _toHexString(int_t value, bool uppercase, bool prefix) noexcept;

template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool> = true>
std::string_view _toOctString(int_t value, bool prefix) noexcept;

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
  ieee754_t num{ .f = value };

  if ((num.w & ieee754_t::C_INFINITY) == ieee754_t::C_INFINITY)
    retval = (num.b.sign == 0U) ? "inf"sv : "-inf"sv;

  else if ((num.w & ieee754_t::C_NAN) == ieee754_t::C_NAN)
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
{
  using uint_t    = typename ieee754_t::uint_t;
  using int_t     = std::make_signed_t<uint_t>;

  int_t exponent = value.b.exponent - ieee754_t::C_BIAS;
  uint_t mantissa = (value.b.fraction >> (ieee754_t::C_FRACTION_WIDTH - exponent)) | uint_t{1} << exponent;

  (void)exponent;
  (void)mantissa;

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
