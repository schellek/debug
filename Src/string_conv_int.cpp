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
using FastSignedInt = ConditionalT<sizeof(T) <= sizeof(int), int, T>;

template <typename T>
using FastUnsignedInt = MakeUnsignedT<FastSignedInt<T>>;

template <typename T>
using FastInt = ConditionalT<IsSignedV<T>, FastSignedInt<T>, FastUnsignedInt<T>>;

template <typename T>
using FastBitPatternInt = ConditionalT<IsSignedV<T>, MakeUnsignedT<T>, FastUnsignedInt<T>>;

template <typename T, EnableIfT<IsIntegralV<T>> = true>
static std::string_view _toString(T value) noexcept;

template <typename T, EnableIfT<IsIntegralV<T>> = true>
static std::string_view _toHexString(T value, bool uppercase, bool prefix) noexcept;

template <typename T, EnableIfT<IsIntegralV<T>> = true>
static std::string_view _toOctString(T value, bool prefix) noexcept;

std::string_view toString(bool value) noexcept
{
  return (value) ? "true"sv : "false"sv;
}

template <typename T, EnableIfT<IsIntegralV<T>>>
std::string_view toString(T value) noexcept
{
  return _toString(static_cast<FastInt<T>>(value));
}

template <typename T, EnableIfT<IsIntegralV<T>>>
std::string_view toHexString(T value, bool uppercase, bool prefix) noexcept
{
  return _toHexString(static_cast<FastBitPatternInt<T>>(value), uppercase, prefix);
}

template <typename T, EnableIfT<IsIntegralV<T>>>
std::string_view toOctString(T value, bool prefix) noexcept
{
  return _toOctString(static_cast<FastBitPatternInt<T>>(value), prefix);
}

template <typename T, EnableIfT<IsIntegralV<T>>>
static std::string_view _toString(T value) noexcept
{
  T nextIterValue;
  bool sign = false;

  char *const end = StringConvBuffer + std::numeric_limits<T>::digits10 + (IsSignedV<T> ? 2U : 1U);
  char *begin = end;

  if constexpr (IsUnsignedV<T>)
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

  if constexpr (IsUnsignedV<T>)
    /* Do nothing */;
  else if (sign)
    *(--begin) = '-';

  return {begin, static_cast<std::string_view::size_type>(end - begin)};
}

template <typename T, EnableIfT<IsIntegralV<T>>>
static std::string_view _toHexString(T value, bool uppercaseLetters, bool prefix) noexcept
{
  const char (&hexLookup)[16U] = HEX_LOOKUP[uppercaseLetters ? 1U : 0U];

  char *const end = StringConvBuffer + ((sizeof(T) * 8U) / 4U) + 2U;
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

template <typename T, EnableIfT<IsIntegralV<T>>>
static std::string_view _toOctString(T value, bool prefix) noexcept
{
  char *const end = StringConvBuffer + ((sizeof(T) * 8U) / 3U) + 2U;
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

#define FMT_INSTANCIATE_TEMPLATE(TYPE) template std::string_view FMT_ABI::toString(TYPE) noexcept
#define FMT_INSTANCIATE_INTEGRAL
#include "fmt/instanciate_template.hpp"
#undef FMT_INSTANCIATE_TEMPLATE

#define FMT_INSTANCIATE_TEMPLATE(TYPE) template std::string_view FMT_ABI::toHexString(TYPE, bool, bool) noexcept
#define FMT_INSTANCIATE_INTEGRAL
#include "fmt/instanciate_template.hpp"
#undef FMT_INSTANCIATE_TEMPLATE

#define FMT_INSTANCIATE_TEMPLATE(TYPE) template std::string_view FMT_ABI::toOctString(TYPE, bool) noexcept
#define FMT_INSTANCIATE_INTEGRAL
#include "fmt/instanciate_template.hpp"
#undef FMT_INSTANCIATE_TEMPLATE
