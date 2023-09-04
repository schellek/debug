#include "fmt/formatted_writer.hpp"

FMT_BEGIN_NAMESPACE

FormattedWriter::FormattedWriter(OStream &stream)
  : OStream{stream}
{
}

FormattedWriter::tSize FormattedWriter::operator()(std::string_view toBeWritten,
                                                      const FormatOptions options,
                                                      const ArgFlag argFlags)
{
  tSize written = 0u;
  int fieldWidth = options.fieldWidth;

  const bool isIntegral = IsFlagSet(argFlags, ArgFlag::Integral);
  const bool isArithmetic = IsFlagSet(argFlags, ArgFlag::Integral | ArgFlag::FloatingPoint);
  const bool isString = IsFlagSet(argFlags, ArgFlag::String);
  const bool isSigned = IsFlagSet(argFlags, ArgFlag::Signed);
  const bool isHexPrefixed = options.hashFlag && IsFlagSet(argFlags, ArgFlag::Hexadecimal);
  const bool isNegative = isSigned && (toBeWritten.at(0) == '-');
  char signChar = '\0';
  std::string_view prefix;

  if (!isSigned)
    /* Do nothing */;
  else if (isNegative)
    signChar = (toBeWritten.remove_prefix(1u), --fieldWidth, '-');
  else if (options.plusFlag)
    signChar = (--fieldWidth, '+');
  else if (options.spaceFlag)
    signChar = (--fieldWidth, ' ');

  if (isHexPrefixed)
  {
    prefix = toBeWritten.substr(0u, 2u);
    toBeWritten.remove_prefix(2u);
    fieldWidth -= 2;
  }

  tSize toBeWrittenSize = static_cast<tSize>(toBeWritten.size());

  if (isString && (options.precision != FormatOptions::NOT_SPECIFIED) && (options.precision < toBeWrittenSize))
  {
    tSize toRemove = static_cast<tSize>(toBeWrittenSize - options.precision);
    toBeWritten.remove_suffix(toRemove);
    toBeWrittenSize -= toRemove;
  }

  tSize subFieldWidth;

  if (isIntegral && (options.precision > toBeWrittenSize))
    subFieldWidth = options.precision;
  else
    subFieldWidth = toBeWrittenSize;

  if (const bool isZero = isIntegral && (toBeWrittenSize == 1u) && (toBeWritten.at(0) == '0');
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
      written += write('0', static_cast<tSize>(subFieldWidth - toBeWrittenSize));

    written += write(toBeWritten);

    if (fieldWidth > written)
      written += write(' ', static_cast<tSize>(fieldWidth - written));
  }
  else if (isArithmetic && (subFieldWidth == toBeWrittenSize) && options.zeroFlag)
  {
    if (signChar != '\0')
      written += write(signChar);

    if (isHexPrefixed)
      written += write(prefix);

    if (fieldWidth > toBeWrittenSize)
      written += write('0', static_cast<tSize>(fieldWidth - toBeWrittenSize));

    written += write(toBeWritten);
  }
  else
  {
    if (fieldWidth > subFieldWidth)
      written += write(' ', static_cast<tSize>(fieldWidth - subFieldWidth));

    if (signChar != '\0')
      written += write(signChar);

    if (isHexPrefixed)
      written += write(prefix);

    if (subFieldWidth > toBeWrittenSize)
      written += write('0', static_cast<tSize>(subFieldWidth - toBeWrittenSize));

    written += write(toBeWritten);
  }

  return written;
}

FMT_END_NAMESPACE
