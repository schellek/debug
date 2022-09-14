#include "fmt/string_conv.hpp"
#include "fmt/string_conv_buffer.hpp"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

using namespace std::string_view_literals;

static constexpr char HEX_LOOKUP[2U][16U] =
{
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' },
  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' }
};

template <typename T>
using FastSignedInt = std::conditional_t<sizeof(T) <= sizeof(int), int, T>;

template <typename T>
using FastUnsignedInt = std::make_unsigned_t<FastSignedInt<T>>;

template <typename T>
using FastInt = std::conditional_t<std::is_signed<T>::value, FastSignedInt<T>, FastUnsignedInt<T>>;

template <typename T>
using FastBitPatternInt = std::conditional_t<std::is_signed<T>::value, std::make_unsigned_t<T>, FastUnsignedInt<T>>;

template <typename int_t, std::enable_if_t<std::is_integral<int_t>::value, bool> = true>
static std::string_view _toString(int_t value) noexcept;

template <typename int_t, std::enable_if_t<std::is_integral<int_t>::value, bool> = true>
static std::string_view _toHexString(int_t value, bool uppercase, bool prefix) noexcept;

template <typename int_t, std::enable_if_t<std::is_integral<int_t>::value, bool> = true>
static std::string_view _toOctString(int_t value, bool prefix) noexcept;

std::string_view toString(bool value) noexcept
{
  return (value) ? "true"sv : "false"sv;
}

template <typename int_t, std::enable_if_t<std::is_integral<int_t>::value, bool>>
std::string_view toString(int_t value) noexcept
{
  return _toString(static_cast<FastInt<int_t>>(value));
}

template <typename int_t, std::enable_if_t<std::is_integral<int_t>::value, bool>>
std::string_view toHexString(int_t value, bool uppercase, bool prefix) noexcept
{
  return _toHexString(static_cast<FastBitPatternInt<int_t>>(value), uppercase, prefix);
}

template <typename int_t, std::enable_if_t<std::is_integral<int_t>::value, bool>>
std::string_view toOctString(int_t value, bool prefix) noexcept
{
  return _toOctString(static_cast<FastBitPatternInt<int_t>>(value), prefix);
}

template <typename int_t, std::enable_if_t<std::is_integral<int_t>::value, bool>>
static std::string_view _toString(int_t value) noexcept
{
  int_t nextIterValue;
  bool sign = false;

  char *const end = StringConvBuffer + std::numeric_limits<int_t>::digits10 + (std::is_signed<int_t>::value ? 2U : 1U);
  char *begin = end;

  if constexpr (std::is_unsigned<int_t>::value)
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

  if constexpr (std::is_unsigned<int_t>::value)
    /* Do nothing */;
  else if (sign)
    *(--begin) = '-';

  return {begin, static_cast<std::string_view::size_type>(end - begin)};
}

template <typename int_t, std::enable_if_t<std::is_integral<int_t>::value, bool>>
static std::string_view _toHexString(int_t value, bool uppercaseLetters, bool prefix) noexcept
{
  const char (&hexLookup)[16U] = HEX_LOOKUP[uppercaseLetters ? 1U : 0U];

  char *const end = StringConvBuffer + ((sizeof(int_t) * 8U) / 4U) + 2U;
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

template <typename int_t, std::enable_if_t<std::is_integral<int_t>::value, bool>>
static std::string_view _toOctString(int_t value, bool prefix) noexcept
{
  char *const end = StringConvBuffer + ((sizeof(int_t) * 8U) / 3U) + 2U;
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

FMT_END_NAMESPACE

#define INSTANCIATE_TEMPLATE(TYPE) template std::string_view FMT_ABI::toString(TYPE) noexcept
#define INSTANCIATE_INTEGRAL
#include "fmt/instanciate_template.hpp"
#undef INSTANCIATE_TEMPLATE

#define INSTANCIATE_TEMPLATE(TYPE) template std::string_view FMT_ABI::toHexString(TYPE, bool, bool) noexcept
#define INSTANCIATE_INTEGRAL
#include "fmt/instanciate_template.hpp"
#undef INSTANCIATE_TEMPLATE

#define INSTANCIATE_TEMPLATE(TYPE) template std::string_view FMT_ABI::toOctString(TYPE, bool) noexcept
#define INSTANCIATE_INTEGRAL
#include "fmt/instanciate_template.hpp"
#undef INSTANCIATE_TEMPLATE
