#include <cstring>
#include <cstdlib>
#include <cctype>

#include "fmt/ostream.hpp"
#include "fmt/string_conv.hpp"


using namespace std::string_view_literals;

namespace fmt
{

static constexpr std::string_view HEX_PREFIX = "0x"sv;

namespace align
{
enum type : uint8_t { right = 0U, left };
}
using align_t = align::type;

namespace trait
{
enum type : uint8_t { regular = 0U, as_char, as_short, as_long, as_long_long, as_invalid };
}
using trait_t = trait::type;

struct format_options
{
  align_t alignment;
  bool outputHexPrefix;
  bool outputSign;
  ostream::size_type fieldWidth;
  char paddingChar;

  void reset();
};

void format_options::reset(void)
{
  alignment = align::right;
  outputHexPrefix = false;
  outputSign = false;
  fieldWidth = 0U;
  paddingChar = ' ';
}

template <typename _Ty>
static inline _Ty getVaArg(va_list argList)
{
  static_assert(std::is_arithmetic_v<std::remove_pointer_t<_Ty>> || std::is_same_v<_Ty, void *>);

  using _Tp = std::conditional_t<std::is_integral_v<_Ty> && (sizeof(_Ty) < sizeof(int)),
                int,
                std::conditional_t<std::is_same_v<_Ty, float>,
                  double,
                  _Ty>>;

  return static_cast<_Ty>(va_arg(argList, _Tp));
}

ostream::ostream(size_type (*write)(const char *, size_type) noexcept) noexcept
  : m_write{*write}
{
  if (write == nullptr)
    exit(EXIT_FAILURE);
}

ostream::size_type ostream::write(char c) noexcept
{
  return m_write(&c, 1);
}

ostream::size_type ostream::write(char c, size_type n) noexcept
{
  constexpr size_type WRITE_CHUNK_SIZE = 20U;
  char toWrite[WRITE_CHUNK_SIZE];
  size_type written = 0U;

  std::fill_n(toWrite, (n < WRITE_CHUNK_SIZE) ? n : WRITE_CHUNK_SIZE, c);

  for (; n > WRITE_CHUNK_SIZE; n -= WRITE_CHUNK_SIZE)
    written += m_write(toWrite, WRITE_CHUNK_SIZE);

  written += m_write(toWrite, n);

  return written;
}

ostream::size_type ostream::write(const char *str, size_type len) noexcept
{
  return m_write(str, len);
}

ostream::size_type ostream::write(const std::string_view &str) noexcept
{
  return m_write(str.data(), static_cast<size_type>(str.size()));
}

int ostream::vprintf(const char *str, va_list argList) noexcept
{
  int written = 0;

  std::string_view toBeWritten;
  char c;
  format_options options;
  trait_t argTrait;


  while (*str != '\0')
  {
    if (*str != '%')
    {
      written += write(*(str++));
      continue;
    }

    ++str;

    toBeWritten = ""sv;
    options.reset();
    argTrait = trait::regular;

continue_formatting:
    if (argTrait == trait::as_invalid)
      continue;

    if (!std::isalpha(*str) || (*str == 'h') || (*str == 'd'))
      goto skip_to_formatting_options;

    if (*str == 'u')
    {
      if (argTrait == trait::regular)
        toBeWritten = toString(getVaArg<unsigned int>(argList));
      else if (argTrait == trait::as_char)
        toBeWritten = toString(getVaArg<unsigned char>(argList));
      else if (argTrait == trait::as_short)
        toBeWritten = toString(getVaArg<unsigned short int>(argList));
      else if (argTrait == trait::as_long)
        toBeWritten = toString(getVaArg<unsigned long int>(argList));
      else if (argTrait == trait::as_long_long)
        toBeWritten = toString(getVaArg<unsigned long long int>(argList));
    }
    else if ((*str == 'd') || (*str == 'i'))
    {
      if (argTrait == trait::regular)
        toBeWritten = toString(getVaArg<int>(argList));
      else if (argTrait == trait::as_char)
        toBeWritten = toString(getVaArg<signed char>(argList));
      else if (argTrait == trait::as_short)
        toBeWritten = toString(getVaArg<short int>(argList));
      else if (argTrait == trait::as_long)
        toBeWritten = toString(getVaArg<long int>(argList));
      else if (argTrait == trait::as_long_long)
        toBeWritten = toString(getVaArg<long long int>(argList));
    }
    else if ((*str == 'x') || (*str == 'X'))
    {
      const bool upperCase = (*str == 'X');

      if (argTrait == trait::regular)
        toBeWritten = toHexString(getVaArg<unsigned int>(argList), upperCase);
      else if (argTrait == trait::as_char)
        toBeWritten = toHexString(getVaArg<unsigned char>(argList), upperCase);
      else if (argTrait == trait::as_short)
        toBeWritten = toHexString(getVaArg<unsigned short int>(argList), upperCase);
      else if (argTrait == trait::as_long)
        toBeWritten = toHexString(getVaArg<unsigned long int>(argList), upperCase);
      else if (argTrait == trait::as_long_long)
        toBeWritten = toHexString(getVaArg<unsigned long long int>(argList), upperCase);
    }
    else if (*str == 'o')
    {
      if (argTrait == trait::regular)
        toBeWritten = toOctString(getVaArg<unsigned int>(argList));
      else if (argTrait == trait::as_char)
        toBeWritten = toOctString(getVaArg<unsigned char>(argList));
      else if (argTrait == trait::as_short)
        toBeWritten = toOctString(getVaArg<unsigned short int>(argList));
      else if (argTrait == trait::as_long)
        toBeWritten = toOctString(getVaArg<unsigned long int>(argList));
      else if (argTrait == trait::as_long_long)
        toBeWritten = toOctString(getVaArg<unsigned long long int>(argList));
    }
    else if (*str == 'p')
    {
      void *addr = getVaArg<void *>(argList);

      if (addr != nullptr)
      {
        options.outputHexPrefix = true;
        toBeWritten = toString(reinterpret_cast<uintptr_t>(addr));
      }
      else
      {
        toBeWritten = "(nil)"sv;
      }
    }
    else if ((*str == 'f') || (*str == 'F'))
    {
      toBeWritten = toString(getVaArg<float>(argList));
    }
    else if (*str == 's')
    {
      char *s = getVaArg<char *>(argList);
      toBeWritten = (s != nullptr) ? std::string_view{s} : "(null)"sv;
    }
    else if (*str == 'c')
    {
      c = getVaArg<char>(argList);
      toBeWritten = std::string_view{&c, 1};
    }
    else if (*str == 'n')
    {
      if (argTrait == trait::regular)
        *getVaArg<int *>(argList) = written;
      else if (argTrait == trait::as_char)
        *getVaArg<signed char *>(argList) = static_cast<signed char>(written);
      else if (argTrait == trait::as_short)
        *getVaArg<signed short *>(argList) = static_cast<signed short>(written);
      else if (argTrait == trait::as_long)
        *getVaArg<long int *>(argList) = static_cast<long int>(written);
      else if (argTrait == trait::as_long_long)
        *getVaArg<long long int *>(argList) = static_cast<long long int>(written);
    }
    else
    {
      written += write(*str);
    }

    if (!toBeWritten.empty())
      written += write_formatted(toBeWritten, options);

    ++str;
    continue;

skip_to_formatting_options:
    if (*str == '%')
    {
      written += write('%');
      continue;
    }
    else if (*str == 'h')
    {
      if (argTrait == trait::regular)
        argTrait = trait::as_short;
      else if (argTrait == trait::as_short)
        argTrait = trait::as_char;
      else
        argTrait = trait::as_invalid;
    }
    else if (*str == 'l')
    {
      if (argTrait == trait::regular)
        argTrait = trait::as_long;
      else if (argTrait == trait::as_long)
        argTrait = trait::as_long_long;
      else
        argTrait = trait::as_invalid;
    }
    else if (*str == '.')
    {
      options.paddingChar = '0';
    }
    else if (*str == '-')
    {
      options.alignment = align::left;
    }
    else if (std::isdigit(*str))
    {
      options.fieldWidth = (options.fieldWidth * 10U) + static_cast<uint8_t>(*str - '0');
    }

    ++str;
    goto continue_formatting;
  }

  return written;
}

int ostream::printf(const char *str, ...) noexcept
{
  va_list argList;
  int retval;

  va_start(argList, str);
  retval = vprintf(str, argList);
  va_end(argList);

  return retval;
}

ostream & ostream::operator<<(std::integral auto value) noexcept
{
  write(toString(value));
  return *this;
}

ostream & ostream::operator<<(std::floating_point auto value) noexcept
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

ostream & ostream::operator<<(char* str) noexcept
{
  write(std::string_view{str});
  return *this;
}

ostream & ostream::operator<<(const char* str) noexcept
{
  write(std::string_view{str});
  return *this;
}

ostream & ostream::operator<<(const std::string_view &str) noexcept
{
  write(str);
  return *this;
}

ostream & ostream::operator<<(const void *addr) noexcept
{
  write(HEX_PREFIX);
  write(toHexString(reinterpret_cast<uintptr_t>(addr), false));
  return *this;
}

ostream & ostream::operator<<(ostream &(&function)(ostream &stream)) noexcept
{
  return function(*this);
}

ostream::size_type ostream::write_formatted(const std::string_view &to_write, const format_options &options) noexcept
{
  size_type toWriteSize = static_cast<size_type>(to_write.size());

  if (options.outputHexPrefix)
    toWriteSize += static_cast<size_type>(HEX_PREFIX.size());

  const size_type fieldWidth = (options.fieldWidth > toWriteSize) ? options.fieldWidth : toWriteSize;
  const size_type amountFillCharacter = fieldWidth - toWriteSize;
  size_type written = 0U;

  if (options.alignment == align::left)
  {
    if (options.outputHexPrefix)
      written += write(HEX_PREFIX);

    written += write(to_write);
    written += write(' ', amountFillCharacter);
  }
  else
  {
    if (options.paddingChar == '0')
    {
      if (options.outputHexPrefix)
        written += write(HEX_PREFIX);
      written += write(options.paddingChar, amountFillCharacter);
    }
    else
    {
      written += write(options.paddingChar, amountFillCharacter);
      if (options.outputHexPrefix)
        written += write(HEX_PREFIX);
    }

    written += write(to_write);
  }

  return written;
}

#define INSTANCIATE_TEMPLATE(__TYPE__) template ostream & ostream::operator<<(__TYPE__) noexcept
#define INSTANCIATE_INTEGRAL
#define INSTANCIATE_FLOATING_POINT
#include "instanciate_template.hpp"
#undef INSTANCIATE_TEMPLATE

} // namespace fmt
