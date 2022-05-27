#pragma once

#include <cstdint>
#include <cstdarg>

#include <string_view>
#include <functional>

#include "fmt/concepts.hpp"


namespace fmt
{

class ostream
{
  std::function<void(const char *, uint16_t)> m_write;

public:
  ostream() = delete;
  ostream(std::function<void(const char *, uint16_t)> write);

  void write(const char *str, uint16_t len);
  void write(const std::string_view &str);
  void vprintf(const char *str, va_list argList);
  void printf(const char *str, ...);

  ostream& operator<<(integral auto value);
  ostream& operator<<(floating_point auto value);
  ostream& operator<<(char value);
  ostream& operator<<(const char* str);
  ostream& operator<<(const std::string_view &str);
  ostream& operator<<(const void *addr);
  ostream& operator<<(void (&function)(ostream& stream));
};

void endl(ostream& stream);

extern ostream cout;

} // namespace fmt

#include "ostream.inl"