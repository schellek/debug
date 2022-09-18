#pragma once

#include <cstdint>
#include <string_view>

#include "fmt/fmt.h"
#include "fmt/va_list.hpp"

FMT_BEGIN_NAMESPACE

enum class Trait : unsigned int
{
  Regular = 0U,
  AsChar,
  AsShort,
  AsLong,
  AsLongLong,
  AsIntmax_t,
  ASSizeT,
  AsPtrdiff_t,
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

const char * ParseFormatOptions(const char *str, VaList &args, FormatOptions &formatOptions) noexcept;
const char * ParseArgTraitment(const char *str, Trait &argTraitment) noexcept;
bool FormattingIsRequired(const FormatOptions &formatOptions) noexcept;

FMT_END_NAMESPACE
