#include <limits>

#include "fmt/string_conv.hpp"
#include "fmt/string_conv_buffer.hpp"
#include "fmt/type_traits.hpp"
#include "fmt/floating_point.hpp"

FMT_BEGIN_NAMESPACE

using namespace std::string_view_literals;

template <typename T, EnableIfT<IsFloatingPointV<T>> = true>
static std::string_view _ToString(FloatingPoint<T> value) noexcept;

template <typename T, EnableIfT<IsFloatingPointV<T>>>
std::string_view ToString(T value) noexcept
{
  std::string_view retval;
  FloatingPoint<T> num{value};

  if (num.isZero())
    retval = (num.b.sign) ? "-0"sv : "0";

  else if (num.isInf())
    retval = (num.b.sign) ? "-inf"sv : "inf"sv;

  else if (num.isNan())
    retval = (num.b.sign) ? "-nan"sv : "nan"sv;

  else
    retval = _ToString(num);

  return retval;
}

template <typename T, EnableIfT<IsFloatingPointV<T>>>
static std::string_view _ToString(FloatingPoint<T> value) noexcept
{
  // using FloatT = typename FloatingPoint<T>::FloatT;
  // using UIntT  = typename FloatingPoint<T>::UIntT;
  using IntT   = typename FloatingPoint<T>::IntT;

  const IntT e = value.exponent();
  static_cast<void>(e);

  return "<float>"sv;
}

FMT_END_NAMESPACE

#define FMT_INSTANCIATE_TEMPLATE(TYPE) template std::string_view FMT_ABI::ToString(TYPE) noexcept
#define FMT_INSTANCIATE_FLOATING_POINT
#include "fmt/instanciate_template.hpp"
#undef FMT_INSTANCIATE_TEMPLATE
