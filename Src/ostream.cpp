#include <cstdlib>
#include <cctype>

#include "fmt/string_conv.hpp"
#include "fmt/va_list.hpp"
#include "fmt/format_options.hpp"
#include "fmt/formatted_writer.hpp"

FMT_BEGIN_NAMESPACE

using namespace std::string_view_literals;
using tSSize = MakeSignedT<size_t>;
using tUPtrDiff = MakeUnsignedT<ptrdiff_t>;

OStream::tWrite::tWrite(tWrite::StaticFunc *f)
{
  staticFunc = f;
  external = (f != nullptr) ? &OStream::staticWrite : &OStream::dummyWrite;
}

OStream::tWrite::tWrite(tWrite::MemberFunc *f)
{
  memberFunc = f;
  external = (f != nullptr) ? &OStream::memberWrite : &OStream::dummyWrite;
}

OStream::tFlush::tFlush(tFlush::StaticFunc *f)
{
  staticFunc = f;
  external = (f != nullptr) ? &OStream::staticFlush : &OStream::dummyFlush;
}

OStream::tFlush::tFlush(tFlush::MemberFunc *f)
{
  memberFunc = f;
  external = (f != nullptr) ? &OStream::memberFlush : &OStream::dummyFlush;
}

OStream::OStream(tWrite::StaticFunc *write, tFlush::StaticFunc *flush)
  : obj_{nullptr}, write_{write}, flush_{flush}
{
}

OStream::OStream(void *obj, tWrite::MemberFunc *write, tFlush::MemberFunc *flush)
  : obj_{obj}, write_{write}, flush_{flush}
{
}

OStream::tSize OStream::write(char c)
{
  return _write(&c, 1u);
}

OStream::tSize OStream::write(char c, tSize n)
{
  constexpr tSize WRITE_CHUNK_SIZE = 20u;
  char toWrite[WRITE_CHUNK_SIZE];
  tSize written = 0u;

  std::fill_n(toWrite, (n < WRITE_CHUNK_SIZE) ? n : WRITE_CHUNK_SIZE, c);

  for (; n > WRITE_CHUNK_SIZE; n -= WRITE_CHUNK_SIZE)
    written += _write(toWrite, WRITE_CHUNK_SIZE);

  written += _write(toWrite, n);

  return written;
}

OStream::tSize OStream::write(const char *str, tSize len)
{
  return ((str != nullptr) && (len > 0u)) ? _write(str, len) : tSize{0};
}

OStream::tSize OStream::write(std::string_view str)
{
  return (!str.empty()) ?_write(str.data(), static_cast<tSize>(str.size())) : tSize{0};
}

void OStream::flush(void)
{
  _flush();
}

int OStream::vprintf(const char *str, va_list args)
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
      else if (argTraitment == Trait::AsIntmaxT)
        toBeWritten = ToString(va.getArg<uintmax_t>());
      else if (argTraitment == Trait::AsSizeT)
        toBeWritten = ToString(va.getArg<size_t>());
      else if (argTraitment == Trait::AsPtrdiffT)
        toBeWritten = ToString(va.getArg<tUPtrDiff>());

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
      else if (argTraitment == Trait::AsIntmaxT)
        toBeWritten = ToString(va.getArg<intmax_t>());
      else if (argTraitment == Trait::AsSizeT)
        toBeWritten = ToString(va.getArg<tSSize>());
      else if (argTraitment == Trait::AsPtrdiffT)
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
      else if (argTraitment == Trait::AsIntmaxT)
        toBeWritten = ToHexString(va.getArg<uintmax_t>(), upperCase, prefix);
      else if (argTraitment == Trait::AsSizeT)
        toBeWritten = ToHexString(va.getArg<size_t>(), upperCase, prefix);
      else if (argTraitment == Trait::AsPtrdiffT)
        toBeWritten = ToHexString(va.getArg<tUPtrDiff>(), upperCase, prefix);

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
      else if (argTraitment == Trait::AsIntmaxT)
        toBeWritten = ToOctString(va.getArg<uintmax_t>(), prefix);
      else if (argTraitment == Trait::AsSizeT)
        toBeWritten = ToOctString(va.getArg<size_t>(), prefix);
      else if (argTraitment == Trait::AsPtrdiffT)
        toBeWritten = ToOctString(va.getArg<tUPtrDiff>(), prefix);

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
      else if (argTraitment == Trait::AsIntmaxT)
        *va.getArg<intmax_t *>() = static_cast<intmax_t>(written);
      else if (argTraitment == Trait::AsSizeT)
        *va.getArg<size_t *>() = static_cast<tSSize>(written);
      else if (argTraitment == Trait::AsPtrdiffT)
        *va.getArg<ptrdiff_t *>() = static_cast<ptrdiff_t>(written);
    }
    else if (*str == '%')
    {
      written += write(*str);
    }
    else
    {
      written += write(formatBegin, static_cast<tSize>(str + 1 - formatBegin));
    }

    if (!toBeWritten.empty())
      written += FormattingIsRequired(formatOptions) ? writeFormatted(toBeWritten, formatOptions, argFlags) :
                                                       write(toBeWritten);

    ++str;
    continue;
  }

  return written;
}

int OStream::printf(const char *str, ...)
{
  va_list args;
  int retval;

  va_start(args, str);
  retval = vprintf(str, args);
  va_end(args);

  return retval;
}

template <typename T, EnableIfT<IsIntegralV<T>>>
OStream & OStream::operator<<(T value)
{
  write(ToString(value));
  return *this;
}

template <typename T, EnableIfT<IsFloatingPointV<T>>>
OStream & OStream::operator<<(T value)
{
  write(ToString(value));
  return *this;
}

OStream & OStream::operator<<(bool value)
{
  write(value ? "true"sv : "false"sv);
  return *this;
}

OStream & OStream::operator<<(char value)
{
  write(value);
  return *this;
}

OStream & OStream::operator<<(char *str)
{
  if (str != nullptr)
    write(std::string_view{str});
  return *this;
}

OStream & OStream::operator<<(const char *str)
{
  if (str != nullptr)
    write(std::string_view{str});
  return *this;
}

OStream & OStream::operator<<(std::string_view str)
{
  write(str);
  return *this;
}

OStream & OStream::operator<<(std::nullptr_t)
{
  write("nullptr"sv);
  return *this;
}

OStream & OStream::operator<<(const void *p)
{
  write(ToHexString(reinterpret_cast<uintptr_t>(p), false, true));
  return *this;
}

OStream & OStream::operator<<(ManipFunc &function)
{
  return function(*this);
}

inline OStream::tSize OStream::_write(const char *str, tSize len)
{
  return (this->*write_.external)(str, len);
}

OStream::tSize OStream::staticWrite(const char *str, tSize len)
{
  return (*write_.staticFunc)(str, len);
}

OStream::tSize OStream::memberWrite(const char *str, tSize len)
{
  return (*write_.memberFunc)(obj_, str, len);
}

OStream::tSize OStream::dummyWrite(const char *str, tSize len)
{
  static_cast<void>(str);
  static_cast<void>(len);
  return 0u;
}

inline void OStream::_flush(void)
{
  (this->*flush_.external)();
}

void OStream::staticFlush(void)
{
  (*flush_.staticFunc)();
}

void OStream::memberFlush(void)
{
  (flush_.memberFunc)(obj_);
}

void OStream::dummyFlush(void)
{
  /* Do nothing */;
}

OStream & Flush(OStream &stream)
{
  stream.flush();

  return stream;
}

OStream & Endl(OStream &stream)
{
  stream.write(FMT_ENDL, static_cast<OStream::tSize>(sizeof(FMT_ENDL) - 1u));
  stream.flush();

  return stream;
}

#define FMT_INSTANCIATE_TEMPLATE(TYPE) template OStream & OStream::operator<<(TYPE)
#define FMT_INSTANCIATE_INTEGRAL
#define FMT_INSTANCIATE_FLOATING_POINT
#include "fmt/instanciate_template.hpp"
#undef FMT_INSTANCIATE_TEMPLATE

FMT_END_NAMESPACE