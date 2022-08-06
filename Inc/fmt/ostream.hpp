#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdarg>

#include <string_view>
#include <type_traits>

#include "fmt/stdout.h"


namespace fmt
{

class ostream
{
public:
  using size_type = fmt_size_type;
  using write_func = size_type(const char *, size_type) noexcept;
  using flush_func = void(void) noexcept;
  using manip_func = ostream &(ostream &) noexcept;

private:
  write_func *const m_write;
  flush_func *const m_flush;

public:
  ostream() = delete;
  ostream(write_func *write, flush_func *flush = nullptr) noexcept;

  size_type write(char c) noexcept;
  size_type write(char c, size_type n) noexcept;
  size_type write(const char *str, size_type len) noexcept;
  size_type write(std::string_view str) noexcept;

  void flush(void) noexcept;

  int vprintf(const char *str, va_list args) noexcept;
  int printf(const char *str, ...) noexcept;

  template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool> = true>
  ostream & operator<<(int_t value) noexcept;

  template <typename float_t, std::enable_if_t<std::is_floating_point_v<float_t>, bool> = true>
  ostream & operator<<(float_t value) noexcept;

  ostream & operator<<(bool value) noexcept;
  ostream & operator<<(char value) noexcept;
  ostream & operator<<(char *str) noexcept;
  ostream & operator<<(const char *str) noexcept;
  ostream & operator<<(std::string_view str) noexcept;

  template <typename T>
  ostream & operator<<(T *value) noexcept;

  ostream & operator<<(const void *addr) noexcept;
  ostream & operator<<(manip_func &function) noexcept;
};

ostream & flush(ostream &stream) noexcept;
ostream & endl(ostream &stream) noexcept;

extern ostream cout;

} // namespace fmt

#include "ostream.inl"
