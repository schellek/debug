#include <limits>

#include "fmt/string_conv.hpp"
#include "fmt/string_conv_buffer.hpp"
#include "fmt/type_traits.hpp"
#include "fmt/floating_point.hpp"

FMT_BEGIN_NAMESPACE

using namespace std::string_view_literals;

template <typename _float_t, std::enable_if_t<std::is_floating_point<_float_t>::value, bool> = true>
static std::string_view _toString(FloatingPoint<_float_t> value) noexcept;

template <typename float_t, std::enable_if_t<std::is_floating_point<float_t>::value, bool>>
std::string_view toString(float_t value) noexcept
{
  std::string_view retval;
  FloatingPoint<float_t> num{value};

  if (num.isZero())
    retval = (num.b.sign) ? "-0"sv : "0";

  else if (num.isInf())
    retval = (num.b.sign) ? "-inf"sv : "inf"sv;

  else if (num.isNan())
    retval = (num.b.sign) ? "-nan"sv : "nan"sv;

  else
    retval = _toString(num);

  return retval;
}

template <typename _float_t, std::enable_if_t<std::is_floating_point<_float_t>::value, bool>>
static std::string_view _toString(FloatingPoint<_float_t> value) noexcept
{
  using float_t = typename FloatingPoint<_float_t>::float_t;
  using uint_t  = typename FloatingPoint<_float_t>::uint_t;
  using int_t   = typename FloatingPoint<_float_t>::int_t;

  const int_t e = value.exponent();
  static_cast<void>(e);

  return "<float>"sv;
}

FMT_END_NAMESPACE

#define FMT_INSTANCIATE_TEMPLATE(TYPE) template std::string_view FMT_ABI::toString(TYPE) noexcept
#define FMT_INSTANCIATE_FLOATING_POINT
#include "fmt/instanciate_template.hpp"
#undef FMT_INSTANCIATE_TEMPLATE
