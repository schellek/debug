#pragma once

#include <cstdint>
#include <string_view>

#include "fmt/fmt.h"
#include "fmt/va_list.hpp"

FMT_BEGIN_NAMESPACE

enum class trait : uint8_t
{
  regular = 0U,
  asChar,
  asShort,
  asLong,
  asLongLong,
  asIntmax_t,
  asSize_t,
  asPtrdiff_t,
  asInvalid
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

const char * parseFormatOptions(const char *str, VaList &args, FormatOptions &formatOptions) noexcept;
const char * parseArgTraitment(const char *str, trait &argTraitment) noexcept;
bool formattingIsRequired(const FormatOptions &formatOptions) noexcept;

FMT_END_NAMESPACE
