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

private:
  size_type (&m_write)(const char *, size_type) noexcept;
  size_type write_formatted(const std::string_view &to_write, const format_options &options) noexcept;

public:
  ostream() = delete;
  ostream(size_type (*write)(const char *, size_type) noexcept) noexcept;

  size_type write(char c) noexcept;
  size_type write(char c, size_type n) noexcept;
  size_type write(const char *str, size_type len) noexcept;
  size_type write(const std::string_view &str) noexcept;
  int vprintf(const char *str, va_list argList) noexcept;
  int printf(const char *str, ...) noexcept;

  ostream & operator<<(std::integral auto value) noexcept;
  ostream & operator<<(std::floating_point auto value) noexcept;
  ostream & operator<<(bool value) noexcept;
  ostream & operator<<(char value) noexcept;
  ostream & operator<<(char* str) noexcept;
  ostream & operator<<(const char* str) noexcept;
  ostream & operator<<(const std::string_view &str) noexcept;

  template <typename _Ty>
  ostream & operator<<(_Ty *value) noexcept;

  ostream & operator<<(const void *addr) noexcept;
  ostream & operator<<(ostream & (&function)(ostream &stream)) noexcept;

  template <typename _Ty, size_t N>
  ostream & operator<<(const std::array<_Ty, N> &array) noexcept;

  template <size_t idx = 0U, typename ..._Ty>
  ostream & operator<<(const std::tuple<_Ty...> &tuple) noexcept;

  template <typename _Ty1, typename _Ty2>
  ostream & operator<<(const std::pair<_Ty1, _Ty2> &pair) noexcept;
};

ostream &endl(ostream &stream) noexcept;

template <typename ..._Ty>
void print(ostream &stream, _Ty && ...value) noexcept;

extern ostream cout;

} // namespace fmt

#include "ostream.inl"
