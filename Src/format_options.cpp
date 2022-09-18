#include <cstring>
#include <cctype>

#include "fmt/format_options.hpp"

FMT_BEGIN_NAMESPACE

static constexpr FormatOptions DEFAULT_FORMATTING =
{
  false, false, false, false, false,
  FormatOptions::NOT_SPECIFIED, FormatOptions::NOT_SPECIFIED
};

static const char * parseFlags(const char *str, FormatOptions &formatOptions) noexcept;
static const char * parseWidth(const char *str, VaList &args, FormatOptions &formatOptions) noexcept;
static const char * parsePrecision(const char *str, VaList &args, FormatOptions &formatOptions) noexcept;
static const char * parseFlag(const char *str, FormatOptions &formatOptions) noexcept;

const char * ParseFormatOptions(const char *str, VaList &args, FormatOptions &formatOptions) noexcept
{
  std::memcpy(&formatOptions, &DEFAULT_FORMATTING, sizeof(FormatOptions));

  str = parseFlags(str, formatOptions);
  str = parseWidth(str, args, formatOptions);
  str = parsePrecision(str, args, formatOptions);

  return str;
}

const char * ParseArgTraitment(const char *str, Trait &argTraitment) noexcept
{
  argTraitment = Trait::Regular;

  while (*str == 'h')
  {
    if (argTraitment == Trait::Regular)
      argTraitment = Trait::AsShort;
    else if (argTraitment == Trait::AsShort)
      argTraitment = Trait::AsChar;
    else
      argTraitment = Trait::AsInvalid;

    ++str;
  }

  while (*str == 'l')
  {
    if (argTraitment == Trait::Regular)
      argTraitment = Trait::AsLong;
    else if (argTraitment == Trait::AsLong)
      argTraitment = Trait::AsLongLong;
    else
      argTraitment = Trait::AsInvalid;

    ++str;
  }

  if (*str == 'j')
    argTraitment = (++str, (argTraitment == Trait::Regular) ? Trait::AsIntmax_t : Trait::AsInvalid);
  else if (*str == 'z')
    argTraitment = (++str, (argTraitment == Trait::Regular) ? Trait::ASSizeT : Trait::AsInvalid);
  else if (*str == 't')
    argTraitment = (++str, (argTraitment == Trait::Regular) ? Trait::AsPtrdiff_t : Trait::AsInvalid);

  return str;
}

bool FormattingIsRequired(const FormatOptions &formatOptions) noexcept
{
  return (std::memcmp(&formatOptions, &DEFAULT_FORMATTING, sizeof(FormatOptions)) != 0);
}

static const char * parseFlags(const char *str, FormatOptions &formatOptions) noexcept
{
  const char *preParse, *postParse = str;

  do
  {
    preParse = postParse;
    postParse = parseFlag(preParse, formatOptions);
  } while (postParse > preParse);

  return postParse;
}

static const char * parseWidth(const char *str, VaList &args, FormatOptions &formatOptions) noexcept
{
  const char *preParse = str;
  int fieldWidth = 0;

  if (*str == '*')
    fieldWidth = (++str, args.getArg<int>());
  else while (std::isdigit(*str))
    fieldWidth = fieldWidth * 10 + *(str++) - '0';

  if (fieldWidth < 0)
  {
    formatOptions.fieldWidth = fieldWidth * -1;
    formatOptions.minusFlag = true;
  }
  else if (str > preParse)
  {
    formatOptions.fieldWidth = fieldWidth;
  }

  return str;
}

static const char * parsePrecision(const char *str, VaList &args, FormatOptions &formatOptions) noexcept
{
  const char *preParse = str;
  int precision = 0;

  if (*str != '.')
  {
    /* Do nothing */;
  }
  else if (++str; *str == '*')
  {
    ++str;
    precision = args.getArg<int>();
  }
  else
  {
    bool sign = (*str == '-') ? (++str, true) : false;

    while (std::isdigit(*str))
      precision = precision * 10 + *(str++) - '0';

    if (sign)
      precision *= -1;
  }

  if (precision < 0)
    formatOptions.precision = FormatOptions::NOT_SPECIFIED;
  else if (str > preParse)
    formatOptions.precision = precision;

  return str;
}

static const char * parseFlag(const char *str, FormatOptions &formatOptions) noexcept
{
  bool flagSpecified = true;

  if (*str == '-')
    formatOptions.minusFlag = true;
  else if (*str == '+')
    formatOptions.plusFlag = true;
  else if (*str == ' ')
    formatOptions.spaceFlag = true;
  else if (*str == '#')
    formatOptions.hashFlag = true;
  else if (*str == '0')
    formatOptions.zeroFlag = true;
  else
    flagSpecified = false;

  if (flagSpecified)
    ++str;

  return str;
}

FMT_END_NAMESPACE
