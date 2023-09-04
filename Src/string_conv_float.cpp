#include <limits>

#include "fmt/string_conv.hpp"
#include "fmt/string_conv_buffer.hpp"
#include "fmt/type_traits.hpp"
#include "fmt/floating_point.hpp"
#include <cmath>

FMT_BEGIN_NAMESPACE

using namespace std::string_view_literals;

template <typename T>
static std::string_view _ToString(FloatingPoint<T> value);

template <typename T, EnableIfT<IsFloatingPointV<T>>>
std::string_view ToString(T value)
{
  std::string_view retval;
  FloatingPoint<T> num{value};

  if (num.isZero())
    retval = (num.b.sign) ? "-0"sv : "0"sv;

  else if (num.isInf())
    retval = (num.b.sign) ? "-inf"sv : "inf"sv;

  else if (num.isNan())
    retval = (num.b.sign) ? "-nan"sv : "nan"sv;

  else
    retval = _ToString(num);

  return retval;
}

template <typename T>
static std::string_view _ToString(FloatingPoint<T> value)
{
  FloatingPoint<T> vPlus = value, vMinus = value;

  if (value.exponent() >= 0)
  {

  }
  else
  {

  }

  if ((value.b.biasedExponent) == 0u /*|| (value.b.mantissa)*/) [[likely]]
  {
    // v- = (f - 1) * b^e

  }
  else
  {

    --vMinus.b.biasedExponent;
    // v- = (b^p - 1) * b^(e - 1)
  }

  // v+ = (f + 1) * b^e

  // f =
  // b = 2
  // e = unbiased exponent
  // v = f * b^e
  //
  // v- = v - b^e         if e == min or f != b^(p - 1)
  //    = v - b^(e - 1)   else
  //
  // v+ = v + b^(e - 1)
  //
  // p is the fixed size of the mantissa in base digits


  (void)vPlus;
  (void)vMinus;

  return "<float>"sv;
}

FMT_END_NAMESPACE

#define FMT_INSTANCIATE_TEMPLATE(TYPE) template std::string_view FMT_ABI::ToString(TYPE)
#define FMT_INSTANCIATE_FLOATING_POINT
#include "fmt/instanciate_template.hpp"
#undef FMT_INSTANCIATE_TEMPLATE
