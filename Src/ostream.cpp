#include <cstdlib>
#include <cctype>

#include "fmt/string_conv.hpp"
#include "fmt/va_list.hpp"
#include "fmt/format_options.hpp"
#include "fmt/formatted_writer.hpp"

FMT_BEGIN_NAMESPACE

using namespace std::string_view_literals;
using SSizeT = MakeSignedT<size_t>;
using UPtrDiffT = MakeUnsignedT<ptrdiff_t>;

OStream::OStream(WriteFunc *write, FlushFunc *flush) noexcept
  : m_write{write}, m_flush{flush}
{
}

OStream::SizeType OStream::write(char c) noexcept
{
  return (*m_write)(&c, 1U);
}

OStream::SizeType OStream::write(char c, SizeType n) noexcept
{
  constexpr SizeType WRITE_CHUNK_SIZE = 20U;
  char toWrite[WRITE_CHUNK_SIZE];
  SizeType written = 0U;

  std::fill_n(toWrite, (n < WRITE_CHUNK_SIZE) ? n : WRITE_CHUNK_SIZE, c);

  for (; n > WRITE_CHUNK_SIZE; n -= WRITE_CHUNK_SIZE)
    written += (*m_write)(toWrite, WRITE_CHUNK_SIZE);

  written += (*m_write)(toWrite, n);

  return written;
}

OStream::SizeType OStream::write(const char *str, SizeType len) noexcept
{
  return ((str != nullptr) && (len > 0U)) ? (*m_write)(str, len) : SizeType{0};
}

OStream::SizeType OStream::write(std::string_view str) noexcept
{
  return (!str.empty()) ? (*m_write)(str.data(), static_cast<SizeType>(str.size())) : SizeType{0};
}

void OStream::flush(void) noexcept
{
  if (m_flush != nullptr)
    (*m_flush)();
}

int OStream::vprintf(const char *str, va_list args) noexcept
{
  int written = 0;

  FormattedWriter writeFormatted{*this};

  const char *formatBegin;
  FormatOptions formatOptions;
  Trait argTraitment;
  std::string_view toBeWritten;
  ArgFlag argFlags;
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

    str = ParseFormatOptions(str, va, formatOptions);
    str = ParseArgTraitment(str, argTraitment);

    if (argTraitment == Trait::AsInvalid)
      continue;

    toBeWritten = ""sv;
    argFlags = ArgFlag::None;

    if (*str == 'u')
    {
      if (argTraitment == Trait::Regular)
        toBeWritten = ToString(va.getArg<unsigned int>());
      else if (argTraitment == Trait::AsChar)
        toBeWritten = ToString(va.getArg<unsigned char>());
      else if (argTraitment == Trait::AsShort)
        toBeWritten = ToString(va.getArg<unsigned short int>());
      else if (argTraitment == Trait::AsLong)
        toBeWritten = ToString(va.getArg<unsigned long int>());
      else if (argTraitment == Trait::AsLongLong)
        toBeWritten = ToString(va.getArg<unsigned long long int>());
      else if (argTraitment == Trait::AsIntmax_t)
        toBeWritten = ToString(va.getArg<uintmax_t>());
      else if (argTraitment == Trait::ASSizeT)
        toBeWritten = ToString(va.getArg<size_t>());
      else if (argTraitment == Trait::AsPtrdiff_t)
        toBeWritten = ToString(va.getArg<UPtrDiffT>());

      argFlags = ArgFlag::Integral | ArgFlag::Decimal;
    }
    else if ((*str == 'd') || (*str == 'i'))
    {
      if (argTraitment == Trait::Regular)
        toBeWritten = ToString(va.getArg<int>());
      else if (argTraitment == Trait::AsChar)
        toBeWritten = ToString(va.getArg<signed char>());
      else if (argTraitment == Trait::AsShort)
        toBeWritten = ToString(va.getArg<short int>());
      else if (argTraitment == Trait::AsLong)
        toBeWritten = ToString(va.getArg<long int>());
      else if (argTraitment == Trait::AsLongLong)
        toBeWritten = ToString(va.getArg<long long int>());
      else if (argTraitment == Trait::AsIntmax_t)
        toBeWritten = ToString(va.getArg<intmax_t>());
      else if (argTraitment == Trait::ASSizeT)
        toBeWritten = ToString(va.getArg<SSizeT>());
      else if (argTraitment == Trait::AsPtrdiff_t)
        toBeWritten = ToString(va.getArg<ptrdiff_t>());

      argFlags = ArgFlag::Signed | ArgFlag::Integral | ArgFlag::Decimal;
    }
    else if ((*str == 'x') || (*str == 'X'))
    {
      const bool upperCase = (*str == 'X');
      const bool prefix = formatOptions.hashFlag;

      if (argTraitment == Trait::Regular)
        toBeWritten = ToHexString(va.getArg<unsigned int>(), upperCase, prefix);
      else if (argTraitment == Trait::AsChar)
        toBeWritten = ToHexString(va.getArg<unsigned char>(), upperCase, prefix);
      else if (argTraitment == Trait::AsShort)
        toBeWritten = ToHexString(va.getArg<unsigned short int>(), upperCase, prefix);
      else if (argTraitment == Trait::AsLong)
        toBeWritten = ToHexString(va.getArg<unsigned long int>(), upperCase, prefix);
      else if (argTraitment == Trait::AsLongLong)
        toBeWritten = ToHexString(va.getArg<unsigned long long int>(), upperCase, prefix);
      else if (argTraitment == Trait::AsIntmax_t)
        toBeWritten = ToHexString(va.getArg<uintmax_t>(), upperCase, prefix);
      else if (argTraitment == Trait::ASSizeT)
        toBeWritten = ToHexString(va.getArg<size_t>(), upperCase, prefix);
      else if (argTraitment == Trait::AsPtrdiff_t)
        toBeWritten = ToHexString(va.getArg<UPtrDiffT>(), upperCase, prefix);

      argFlags = ArgFlag::Integral | ArgFlag::Hexadecimal;
    }
    else if (*str == 'o')
    {
      const bool prefix = formatOptions.hashFlag;

      if (argTraitment == Trait::Regular)
        toBeWritten = ToOctString(va.getArg<unsigned int>(), prefix);
      else if (argTraitment == Trait::AsChar)
        toBeWritten = ToOctString(va.getArg<unsigned char>(), prefix);
      else if (argTraitment == Trait::AsShort)
        toBeWritten = ToOctString(va.getArg<unsigned short int>(), prefix);
      else if (argTraitment == Trait::AsLong)
        toBeWritten = ToOctString(va.getArg<unsigned long int>(), prefix);
      else if (argTraitment == Trait::AsLongLong)
        toBeWritten = ToOctString(va.getArg<unsigned long long int>(), prefix);
      else if (argTraitment == Trait::AsIntmax_t)
        toBeWritten = ToOctString(va.getArg<uintmax_t>(), prefix);
      else if (argTraitment == Trait::ASSizeT)
        toBeWritten = ToOctString(va.getArg<size_t>(), prefix);
      else if (argTraitment == Trait::AsPtrdiff_t)
        toBeWritten = ToOctString(va.getArg<UPtrDiffT>(), prefix);

      argFlags = ArgFlag::Integral | ArgFlag::Octal;
    }
    else if (*str == 'p')
    {
      if (void *p = va.getArg<void *>(); p != nullptr)
      {
        toBeWritten = ToHexString(reinterpret_cast<uintptr_t>(p), false, true);
        argFlags = ArgFlag::Integral | ArgFlag::Hexadecimal;
      }
      else
      {
        toBeWritten = "(nil)"sv;
      }
    }
    else if ((*str == 'f') || (*str == 'F'))
    {
      toBeWritten = ToString(va.getArg<float>());

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
      if (argTraitment == Trait::Regular)
        *va.getArg<int *>() = written;
      else if (argTraitment == Trait::AsChar)
        *va.getArg<signed char *>() = static_cast<signed char>(written);
      else if (argTraitment == Trait::AsShort)
        *va.getArg<signed short *>() = static_cast<signed short>(written);
      else if (argTraitment == Trait::AsLong)
        *va.getArg<long int *>() = static_cast<long int>(written);
      else if (argTraitment == Trait::AsLongLong)
        *va.getArg<long long int *>() = static_cast<long long int>(written);
      else if (argTraitment == Trait::AsIntmax_t)
        *va.getArg<intmax_t *>() = static_cast<intmax_t>(written);
      else if (argTraitment == Trait::ASSizeT)
        *va.getArg<size_t *>() = static_cast<SSizeT>(written);
      else if (argTraitment == Trait::AsPtrdiff_t)
        *va.getArg<ptrdiff_t *>() = static_cast<ptrdiff_t>(written);
    }
    else if (*str == '%')
    {
      written += write(*str);
    }
    else
    {
      written += write(formatBegin, static_cast<SizeType>(str + 1 - formatBegin));
    }

    if (!toBeWritten.empty())
      written += FormattingIsRequired(formatOptions) ? writeFormatted(toBeWritten, formatOptions, argFlags) :
                                                       write(toBeWritten);

    ++str;
    continue;
  }

  return written;
}

int OStream::printf(const char *str, ...) noexcept
{
  va_list args;
  int retval;

  va_start(args, str);
  retval = vprintf(str, args);
  va_end(args);

  return retval;
}

template <typename T, EnableIfT<IsIntegralV<T>>>
OStream & OStream::operator<<(T value) noexcept
{
  write(ToString(value));
  return *this;
}

template <typename T, EnableIfT<IsFloatingPointV<T>>>
OStream & OStream::operator<<(T value) noexcept
{
  write(ToString(value));
  return *this;
}

OStream & OStream::operator<<(bool value) noexcept
{
  write(value ? "true"sv : "false"sv);
  return *this;
}

OStream & OStream::operator<<(char value) noexcept
{
  write(value);
  return *this;
}

OStream & OStream::operator<<(char *str) noexcept
{
  if (str != nullptr)
    write(std::string_view{str});
  return *this;
}

OStream & OStream::operator<<(const char *str) noexcept
{
  if (str != nullptr)
    write(std::string_view{str});
  return *this;
}

OStream & OStream::operator<<(std::string_view str) noexcept
{
  write(str);
  return *this;
}

OStream & OStream::operator<<(std::nullptr_t) noexcept
{
  write("nullptr"sv);
  return *this;
}

OStream & OStream::operator<<(const void *p) noexcept
{
  write(ToHexString(reinterpret_cast<uintptr_t>(p), false, true));
  return *this;
}

OStream & OStream::operator<<(ManipFunc &function) noexcept
{
  return function(*this);
}

OStream & Flush(OStream &stream) noexcept
{
  stream.flush();

  return stream;
}

OStream & Endl(OStream &stream) noexcept
{
  stream.write(FMT_ENDL, static_cast<OStream::SizeType>(sizeof(FMT_ENDL) - 1U));
  stream.flush();

  return stream;
}

#define FMT_INSTANCIATE_TEMPLATE(TYPE) template OStream & OStream::operator<<(TYPE) noexcept
#define FMT_INSTANCIATE_INTEGRAL
#define FMT_INSTANCIATE_FLOATING_POINT
#include "fmt/instanciate_template.hpp"
#undef FMT_INSTANCIATE_TEMPLATE

FMT_END_NAMESPACE