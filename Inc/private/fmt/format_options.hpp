#ifndef FMT_FORMAT_OPTIONS_HPP
#define FMT_FORMAT_OPTIONS_HPP

#include <cstdint>
#include <string_view>

#include "fmt/fmt.h"
#include "fmt/va_list.hpp"

FMT_BEGIN_NAMESPACE

enum class Trait : unsigned int
{
  Regular = 0u,
  AsChar,
  AsShort,
  AsLong,
  AsLongLong,
  AsIntmaxT,
  AsSizeT,
  AsPtrdiffT,
  AsInvalid
};

struct FormatOptions
{
  static constexpr int NOT_SPECIFIED = -1;

  bool minusFlag;
  bool plusFlag;
  bool spaceFlag;
  bool hashFlag;
  bool zeroFlag;

  int fieldWidth;
  int precision;
};

const char * ParseFormatOptions(const char *str, VaList &args, FormatOptions &formatOptions);
const char * ParseArgTraitment(const char *str, Trait &argTraitment);
bool FormattingIsRequired(const FormatOptions &formatOptions);

FMT_END_NAMESPACE

#endif /* FMT_FORMAT_OPTIONS_HPP */