#pragma once

#include "fmt/fmt.h"
#include "fmt/ostream.hpp"
#include "fmt/format_options.hpp"

FMT_BEGIN_NAMESPACE

using ArgFlagBaseT = unsigned int;

enum class ArgFlag : ArgFlagBaseT
{
  None          = 0U,

  Integral      = 1U << 0U,
  FloatingPoint = 1U << 1U,

  String        = 1U << 2U,

  Signed        = 1U << 3U,

  Decimal       = 1U << 4U,
  Octal         = 1U << 5U,
  Hexadecimal   = 1U << 6U
};

static inline constexpr ArgFlag operator|(ArgFlag lhs, ArgFlag rhs) noexcept
{
  return ArgFlag{static_cast<ArgFlagBaseT>(lhs) | static_cast<ArgFlagBaseT>(rhs)};
}

static inline constexpr ArgFlag operator&(ArgFlag lhs, ArgFlag rhs) noexcept
{
  return ArgFlag{static_cast<ArgFlagBaseT>(lhs) & static_cast<ArgFlagBaseT>(rhs)};
}

static inline constexpr bool IsFlagSet(ArgFlag flags, ArgFlag flag) noexcept
{
  return (flags & flag) != ArgFlag::None;
}

struct FormattedWriter : public OStream
{
  FormattedWriter(OStream &stream) noexcept;
  SizeType operator()(std::string_view toBeWritten, const FormatOptions options, const ArgFlag argFlags) noexcept;
};

FMT_END_NAMESPACE
