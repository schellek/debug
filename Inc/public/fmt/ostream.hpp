#pragma once

#include <cstdarg>
#include <string_view>

#include "fmt/fmt.h"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

class OStream
{
public:
  using SizeType  = FmtSizeType;
  using WriteFunc = SizeType(const char *, SizeType) noexcept;
  using FlushFunc = void(void) noexcept;
  using ManipFunc = OStream &(OStream &) noexcept;

  using size_type = SizeType;

private:
  WriteFunc *const m_write;
  FlushFunc *const m_flush;

public:
  OStream() = delete;
  OStream(WriteFunc *write, FlushFunc *flush = nullptr) noexcept;

  SizeType write(char c) noexcept;
  SizeType write(char c, SizeType n) noexcept;
  SizeType write(const char *str, SizeType len) noexcept;
  SizeType write(std::string_view str) noexcept;

  void flush(void) noexcept;

  int vprintf(const char *str, va_list args) noexcept;
  int printf(FMT_PRINTF_FMTSTR const char *str, ...) noexcept FMT_PRINTF_FUNC(2);

  OStream & operator<<(bool value) noexcept;
  OStream & operator<<(char value) noexcept;
  OStream & operator<<(char *str) noexcept;
  OStream & operator<<(const char *str) noexcept;
  OStream & operator<<(std::string_view str) noexcept;
  OStream & operator<<(std::nullptr_t) noexcept;

  template <typename T, EnableIfT<IsIntegralV<T>> = true>
  OStream & operator<<(T value) noexcept;

  template <typename T, EnableIfT<IsFloatingPointV<T>> = true>
  OStream & operator<<(T value) noexcept;

  template <typename T, EnableIfT<IsPointerV<T>> = true>
  OStream & operator<<(T value) noexcept;

  template <typename T, EnableIfT<IsSmartPointerV<T>> = true>
  OStream & operator<<(T &value) noexcept;

  OStream & operator<<(const void *p) noexcept;
  OStream & operator<<(ManipFunc &function) noexcept;
};

OStream & Flush(OStream &stream) noexcept;
OStream & Endl(OStream &stream) noexcept;

using ostream = OStream;
static inline constexpr OStream::ManipFunc &flush = Flush;
static inline constexpr OStream::ManipFunc &endl  = Endl;

extern OStream cout;

FMT_END_NAMESPACE

#include "ostream.inl"
