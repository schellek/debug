#pragma once

#include "fmt/ostream.hpp"
#include "fmt/format_options.hpp"

namespace fmt
{

namespace ArgFlag
{

constexpr uint8_t Integral      = 1U << 0U;
constexpr uint8_t FloatingPoint = 1U << 1U;

constexpr uint8_t String        = 1U << 2U;

constexpr uint8_t Signed        = 1U << 3U;

constexpr uint8_t Decimal       = 1U << 4U;
constexpr uint8_t Octal         = 1U << 5U;
constexpr uint8_t Hexadecimal   = 1U << 6U;

} // namespace ArgFlag

struct FormattedWriter : public ostream
{
  FormattedWriter(ostream &stream) noexcept;
  size_type operator()(std::string_view toBeWritten, const FormatOptions options, const uint8_t argFlags) noexcept;
};

} // namespace fmt
