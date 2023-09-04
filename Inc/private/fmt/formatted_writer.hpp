#ifndef FMT_FORMATTED_WRITER_HPP
#define FMT_FORMATTED_WRITER_HPP

#include "fmt/fmt.h"
#include "fmt/ostream.hpp"
#include "fmt/format_options.hpp"

FMT_BEGIN_NAMESPACE

using ArgFlagBaseT = unsigned int;

enum class ArgFlag : ArgFlagBaseT
{
  None          = 0u,

  Integral      = 1u << 0u,
  FloatingPoint = 1u << 1u,

  String        = 1u << 2u,

  Signed        = 1u << 3u,

  Decimal       = 1u << 4u,
  Octal         = 1u << 5u,
  Hexadecimal   = 1u << 6u
};

static inline constexpr ArgFlag operator|(ArgFlag lhs, ArgFlag rhs)
{
  return ArgFlag{static_cast<ArgFlagBaseT>(lhs) | static_cast<ArgFlagBaseT>(rhs)};
}

static inline constexpr ArgFlag operator&(ArgFlag lhs, ArgFlag rhs)
{
  return ArgFlag{static_cast<ArgFlagBaseT>(lhs) & static_cast<ArgFlagBaseT>(rhs)};
}

static inline constexpr bool IsFlagSet(ArgFlag flags, ArgFlag flag)
{
  return (flags & flag) != ArgFlag::None;
}

struct FormattedWriter : public OStream
{
  FormattedWriter(OStream &stream);
  tSize operator()(std::string_view toBeWritten, const FormatOptions options, const ArgFlag argFlags);
};

FMT_END_NAMESPACE

#endif /* FMT_FORMATTED_WRITER_HPP */