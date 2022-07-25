#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdarg>

#include <string_view>
#include <array>
#include <tuple>

#include <concepts>


namespace fmt
{

struct format_options;

class ostream
{
public:
  using size_type = uint16_t;
  using write_func = size_type(const char *, size_type) noexcept;
  using manip_func = ostream &(ostream &) noexcept;
  using flush_func = void(void) noexcept;

private:
  write_func *const m_write;
  flush_func *const m_flush;
  size_type write_formatted(const std::string_view &to_write, const format_options &options) noexcept;

public:
  ostream() = delete;
  ostream(write_func *write, flush_func *flush = nullptr) noexcept;

  size_type write(char c) noexcept;
  size_type write(char c, size_type n) noexcept;
  size_type write(const char *str, size_type len) noexcept;
  size_type write(const std::string_view &str) noexcept;

  void flush(void) noexcept;

  int vprintf(const char *str, va_list argList) noexcept;
  int printf(const char *str, ...) noexcept;

  ostream & operator<<(std::integral auto value) noexcept;
  ostream & operator<<(std::floating_point auto value) noexcept;
  ostream & operator<<(bool value) noexcept;
  ostream & operator<<(char value) noexcept;
  ostream & operator<<(char* str) noexcept;
  ostream & operator<<(const char* str) noexcept;
  ostream & operator<<(const std::string_view &str) noexcept;

  template <typename T>
  ostream & operator<<(T *value) noexcept;

  ostream & operator<<(const void *addr) noexcept;
  ostream & operator<<(manip_func &function) noexcept;

  template <typename T, size_t N>
  ostream & operator<<(const std::array<T, N> &array) noexcept;

  template <size_t idx = 0U, typename ...T>
  ostream & operator<<(const std::tuple<T...> &tuple) noexcept;

  template <typename T1, typename T2>
  ostream & operator<<(const std::pair<T1, T2> &pair) noexcept;
};

ostream & flush(ostream &stream) noexcept;
ostream & endl(ostream &stream) noexcept;

template <typename ...T>
void print(ostream &stream, T && ...value) noexcept;

extern ostream cout;

} // namespace fmt

#include "ostream.inl"
