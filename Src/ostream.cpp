#include <cstdlib>
#include <cctype>

#include "fmt/string_conv.hpp"
#include "fmt/va_list.hpp"
#include "fmt/format_options.hpp"
#include "fmt/formatted_writer.hpp"

FMT_BEGIN_NAMESPACE

using namespace std::string_view_literals;
using ssize_t = MakeSignedT<size_t>;
using uptrdiff_t = MakeUnsignedT<ptrdiff_t>;

ostream::ostream(write_func *write, flush_func *flush) noexcept
  : m_write{write}, m_flush{flush}
{
}

ostream::size_type ostream::write(char c) noexcept
{
  return (*m_write)(&c, 1U);
}

ostream::size_type ostream::write(char c, size_type n) noexcept
{
  constexpr size_type WRITE_CHUNK_SIZE = 20U;
  char toWrite[WRITE_CHUNK_SIZE];
  size_type written = 0U;

  std::fill_n(toWrite, (n < WRITE_CHUNK_SIZE) ? n : WRITE_CHUNK_SIZE, c);

  for (; n > WRITE_CHUNK_SIZE; n -= WRITE_CHUNK_SIZE)
    written += (*m_write)(toWrite, WRITE_CHUNK_SIZE);

  written += (*m_write)(toWrite, n);

  return written;
}

ostream::size_type ostream::write(const char *str, size_type len) noexcept
{
  return ((str != nullptr) && (len > 0U)) ? (*m_write)(str, len) : size_type{0};
}

ostream::size_type ostream::write(std::string_view str) noexcept
{
  return (!str.empty()) ? (*m_write)(str.data(), static_cast<size_type>(str.size())) : size_type{0};
}

void ostream::flush(void) noexcept
{
  if (m_flush != nullptr)
    (*m_flush)();
}

int ostream::vprintf(const char *str, va_list args) noexcept
{
  int written = 0;

  FormattedWriter writeFormatted{*this};

  const char *formatBegin;
  FormatOptions formatOptions;
  trait argTraitment;
  std::string_view toBeWritten;
  uint8_t argFlags;
  char c;

  VaList va{args};

  while (*str != '\0')
  {
    if (*str != '%')
    {
      written += write(*(str++));
      continue;
    }

    formatBegin = str++;

    str = parseFormatOptions(str, va, formatOptions);
    str = parseArgTraitment(str, argTraitment);

    if (argTraitment == trait::asInvalid)
      continue;

    toBeWritten = ""sv;
    argFlags = 0U;

    if (*str == 'u')
    {
      if (argTraitment == trait::regular)
        toBeWritten = toString(va.getArg<unsigned int>());
      else if (argTraitment == trait::asChar)
        toBeWritten = toString(va.getArg<unsigned char>());
      else if (argTraitment == trait::asShort)
        toBeWritten = toString(va.getArg<unsigned short int>());
      else if (argTraitment == trait::asLong)
        toBeWritten = toString(va.getArg<unsigned long int>());
      else if (argTraitment == trait::asLongLong)
        toBeWritten = toString(va.getArg<unsigned long long int>());
      else if (argTraitment == trait::asIntmax_t)
        toBeWritten = toString(va.getArg<uintmax_t>());
      else if (argTraitment == trait::asSize_t)
        toBeWritten = toString(va.getArg<size_t>());
      else if (argTraitment == trait::asPtrdiff_t)
        toBeWritten = toString(va.getArg<uptrdiff_t>());

      argFlags = ArgFlag::Integral | ArgFlag::Decimal;
    }
    else if ((*str == 'd') || (*str == 'i'))
    {
      if (argTraitment == trait::regular)
        toBeWritten = toString(va.getArg<int>());
      else if (argTraitment == trait::asChar)
        toBeWritten = toString(va.getArg<signed char>());
      else if (argTraitment == trait::asShort)
        toBeWritten = toString(va.getArg<short int>());
      else if (argTraitment == trait::asLong)
        toBeWritten = toString(va.getArg<long int>());
      else if (argTraitment == trait::asLongLong)
        toBeWritten = toString(va.getArg<long long int>());
      else if (argTraitment == trait::asIntmax_t)
        toBeWritten = toString(va.getArg<intmax_t>());
      else if (argTraitment == trait::asSize_t)
        toBeWritten = toString(va.getArg<ssize_t>());
      else if (argTraitment == trait::asPtrdiff_t)
        toBeWritten = toString(va.getArg<ptrdiff_t>());

      argFlags = ArgFlag::Signed | ArgFlag::Integral | ArgFlag::Decimal;
    }
    else if ((*str == 'x') || (*str == 'X'))
    {
      const bool upperCase = (*str == 'X');
      const bool prefix = formatOptions.hashFlag;

      if (argTraitment == trait::regular)
        toBeWritten = toHexString(va.getArg<unsigned int>(), upperCase, prefix);
      else if (argTraitment == trait::asChar)
        toBeWritten = toHexString(va.getArg<unsigned char>(), upperCase, prefix);
      else if (argTraitment == trait::asShort)
        toBeWritten = toHexString(va.getArg<unsigned short int>(), upperCase, prefix);
      else if (argTraitment == trait::asLong)
        toBeWritten = toHexString(va.getArg<unsigned long int>(), upperCase, prefix);
      else if (argTraitment == trait::asLongLong)
        toBeWritten = toHexString(va.getArg<unsigned long long int>(), upperCase, prefix);
      else if (argTraitment == trait::asIntmax_t)
        toBeWritten = toHexString(va.getArg<uintmax_t>(), upperCase, prefix);
      else if (argTraitment == trait::asSize_t)
        toBeWritten = toHexString(va.getArg<size_t>(), upperCase, prefix);
      else if (argTraitment == trait::asPtrdiff_t)
        toBeWritten = toHexString(va.getArg<uptrdiff_t>(), upperCase, prefix);

      argFlags = ArgFlag::Integral | ArgFlag::Hexadecimal;
    }
    else if (*str == 'o')
    {
      const bool prefix = formatOptions.hashFlag;

      if (argTraitment == trait::regular)
        toBeWritten = toOctString(va.getArg<unsigned int>(), prefix);
      else if (argTraitment == trait::asChar)
        toBeWritten = toOctString(va.getArg<unsigned char>(), prefix);
      else if (argTraitment == trait::asShort)
        toBeWritten = toOctString(va.getArg<unsigned short int>(), prefix);
      else if (argTraitment == trait::asLong)
        toBeWritten = toOctString(va.getArg<unsigned long int>(), prefix);
      else if (argTraitment == trait::asLongLong)
        toBeWritten = toOctString(va.getArg<unsigned long long int>(), prefix);
      else if (argTraitment == trait::asIntmax_t)
        toBeWritten = toOctString(va.getArg<uintmax_t>(), prefix);
      else if (argTraitment == trait::asSize_t)
        toBeWritten = toOctString(va.getArg<size_t>(), prefix);
      else if (argTraitment == trait::asPtrdiff_t)
        toBeWritten = toOctString(va.getArg<uptrdiff_t>(), prefix);

      argFlags = ArgFlag::Integral | ArgFlag::Octal;
    }
    else if (*str == 'p')
    {
      if (void *p = va.getArg<void *>(); p != nullptr)
      {
        toBeWritten = toHexString(reinterpret_cast<uintptr_t>(p), false, true);
        argFlags = ArgFlag::Integral | ArgFlag::Hexadecimal;
      }
      else
      {
        toBeWritten = "(nil)"sv;
      }
    }
    else if ((*str == 'f') || (*str == 'F'))
    {
      toBeWritten = toString(va.getArg<float>());

      argFlags = ArgFlag::Signed | ArgFlag::FloatingPoint;
    }
    else if (*str == 's')
    {
      char *s = va.getArg<char *>();
      toBeWritten = (s != nullptr) ? std::string_view{s} : "(null)"sv;
      argFlags = ArgFlag::String;
    }
    else if (*str == 'c')
    {
      c = va.getArg<char>();
      toBeWritten = std::string_view{&c, 1};
    }
    else if (*str == 'n')
    {
      if (argTraitment == trait::regular)
        *va.getArg<int *>() = written;
      else if (argTraitment == trait::asChar)
        *va.getArg<signed char *>() = static_cast<signed char>(written);
      else if (argTraitment == trait::asShort)
        *va.getArg<signed short *>() = static_cast<signed short>(written);
      else if (argTraitment == trait::asLong)
        *va.getArg<long int *>() = static_cast<long int>(written);
      else if (argTraitment == trait::asLongLong)
        *va.getArg<long long int *>() = static_cast<long long int>(written);
      else if (argTraitment == trait::asIntmax_t)
        *va.getArg<intmax_t *>() = static_cast<intmax_t>(written);
      else if (argTraitment == trait::asSize_t)
        *va.getArg<size_t *>() = static_cast<ssize_t>(written);
      else if (argTraitment == trait::asPtrdiff_t)
        *va.getArg<ptrdiff_t *>() = static_cast<ptrdiff_t>(written);
    }
    else if (*str == '%')
    {
      written += write(*str);
    }
    else
    {
      written += write(formatBegin, static_cast<size_type>(str + 1 - formatBegin));
    }

    if (!toBeWritten.empty())
      written += formattingIsRequired(formatOptions) ? writeFormatted(toBeWritten, formatOptions, argFlags) :
                                                       write(toBeWritten);

    ++str;
    continue;
  }

  return written;
}

int ostream::printf(const char *str, ...) noexcept
{
  va_list args;
  int retval;

  va_start(args, str);
  retval = vprintf(str, args);
  va_end(args);

  return retval;
}

template <typename T, EnableIfT<IsIntegralV<T>>>
ostream & ostream::operator<<(T value) noexcept
{
  write(toString(value));
  return *this;
}

template <typename T, EnableIfT<IsFloatingPointV<T>>>
ostream & ostream::operator<<(T value) noexcept
{
  write(toString(value));
  return *this;
}

ostream & ostream::operator<<(bool value) noexcept
{
  write(value ? "true"sv : "false"sv);
  return *this;
}

ostream & ostream::operator<<(char value) noexcept
{
  write(value);
  return *this;
}

ostream & ostream::operator<<(char *str) noexcept
{
  if (str != nullptr)
    write(std::string_view{str});
  return *this;
}

ostream & ostream::operator<<(const char *str) noexcept
{
  if (str != nullptr)
    write(std::string_view{str});
  return *this;
}

ostream & ostream::operator<<(std::string_view str) noexcept
{
  write(str);
  return *this;
}

ostream & ostream::operator<<(std::nullptr_t) noexcept
{
  write("nullptr"sv);
  return *this;
}

ostream & ostream::operator<<(const void *p) noexcept
{
  write(toHexString(reinterpret_cast<uintptr_t>(p), false, true));
  return *this;
}

ostream & ostream::operator<<(manip_func &function) noexcept
{
  return function(*this);
}

ostream & flush(ostream &stream) noexcept
{
  stream.flush();

  return stream;
}

ostream & endl(ostream &stream) noexcept
{
  stream.write(FMT_ENDL, static_cast<ostream::size_type>(sizeof(FMT_ENDL) - 1U));
  stream.flush();

  return stream;
}

#define FMT_INSTANCIATE_TEMPLATE(TYPE) template ostream & ostream::operator<<(TYPE) noexcept
#define FMT_INSTANCIATE_INTEGRAL
#define FMT_INSTANCIATE_FLOATING_POINT
#include "fmt/instanciate_template.hpp"
#undef FMT_INSTANCIATE_TEMPLATE

FMT_END_NAMESPACE