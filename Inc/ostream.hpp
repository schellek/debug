#pragma once

#include <cstdint>
#include <cstdarg>
#include <string_view>

namespace debug
{

class ostream
{
  void (&_write)(const char *str, uint16_t len);

public:
  ostream() = delete;
  ostream(void (*_write)(const char *str, uint16_t len));

  void write(const char *str, uint16_t len);
  void write(std::string_view &str);
  void write(std::string_view &&str);
  void vprintf(const char *str, va_list argList);
  void printf(const char *str, ...);

  ostream& operator<<(uint8_t value);
  ostream& operator<<(uint16_t value);
  ostream& operator<<(uint32_t value);
  ostream& operator<<(uint64_t value);
  ostream& operator<<(int8_t value);
  ostream& operator<<(int16_t value);
  ostream& operator<<(int32_t value);
  ostream& operator<<(int64_t value);
  ostream& operator<<(float value);
  ostream& operator<<(char value);
  ostream& operator<<(const char* str);
  ostream& operator<<(const void *addr);
  ostream& operator<<(void (&function)(ostream& stream));
};

void endl(ostream& stream);

extern ostream cout;

} // debug