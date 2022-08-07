#include "formatted_writer.hpp"

#define IS_FLAG_SET(BITFIELD, FLAG)  (((BITFIELD) & (FLAG)) != 0)

namespace fmt
{

FormattedWriter::FormattedWriter(ostream &stream) noexcept
  : ostream{stream}
{
}

FormattedWriter::size_type FormattedWriter::operator()(std::string_view toBeWritten,
                                                       const FormatOptions options,
                                                       const uint8_t argFlags) noexcept
{
  size_type written = 0U;
  int fieldWidth = options.fieldWidth;

  const bool isIntegral = IS_FLAG_SET(argFlags, ArgFlag::Integral);
  const bool isArithmetic = IS_FLAG_SET(argFlags, ArgFlag::Integral | ArgFlag::FloatingPoint);
  const bool isString = IS_FLAG_SET(argFlags, ArgFlag::String);
  const bool isSigned = IS_FLAG_SET(argFlags, ArgFlag::Signed);
  const bool isHexPrefixed = options.hashFlag && IS_FLAG_SET(argFlags, ArgFlag::Hexadecimal);
  const bool isNegative = isSigned && (toBeWritten.at(0) == '-');
  char signChar = '\0';
  std::string_view prefix;

  if (!isSigned)
    /* Do nothing */;
  else if (isNegative)
    signChar = (toBeWritten.remove_prefix(1U), --fieldWidth, '-');
  else if (options.plusFlag)
    signChar = (--fieldWidth, '+');
  else if (options.spaceFlag)
    signChar = (--fieldWidth, ' ');

  if (isHexPrefixed)
  {
    prefix = toBeWritten.substr(0U, 2U);
    toBeWritten.remove_prefix(2U);
    fieldWidth -= 2;
  }

  size_type toBeWrittenSize = toBeWritten.size();

  if (isString && (options.precision != FormatOptions::NOT_SPECIFIED) && (options.precision < toBeWrittenSize))
  {
    size_type toRemove = static_cast<size_type>(toBeWrittenSize - options.precision);
    toBeWritten.remove_suffix(toRemove);
    toBeWrittenSize -= toRemove;
  }

  size_type subFieldWidth;

  if (isIntegral && (options.precision > toBeWrittenSize))
    subFieldWidth = options.precision;
  else
    subFieldWidth = toBeWrittenSize;

  if (const bool isZero = isIntegral && (toBeWrittenSize == 1U) && (toBeWritten.at(0) == '0');
      isZero && (options.precision == 0))
  {
    /* Do nothing */;
  }
  else if (options.minusFlag)
  {
    if (signChar != '\0')
      written += write(signChar);

    if (isHexPrefixed)
      written += write(prefix);

    if (subFieldWidth > toBeWrittenSize)
      written += write('0', static_cast<size_type>(subFieldWidth - toBeWrittenSize));

    written += write(toBeWritten);

    if (fieldWidth > written)
      written += write(' ', static_cast<size_type>(fieldWidth - written));
  }
  else if (isArithmetic && (subFieldWidth == toBeWrittenSize) && options.zeroFlag)
  {
    if (signChar != '\0')
      written += write(signChar);

    if (isHexPrefixed)
      written += write(prefix);

    if (fieldWidth > toBeWrittenSize)
      written += write('0', static_cast<size_type>(fieldWidth - toBeWrittenSize));

    written += write(toBeWritten);
  }
  else
  {
    if (fieldWidth > subFieldWidth)
      written += write(' ', static_cast<size_type>(fieldWidth - subFieldWidth));

    if (signChar != '\0')
      written += write(signChar);

    if (isHexPrefixed)
      written += write(prefix);

    if (subFieldWidth > toBeWrittenSize)
      written += write('0', static_cast<size_type>(subFieldWidth - toBeWrittenSize));

    written += write(toBeWritten);
  }

  return written;
}

} // namespace fmt
