#pragma once

#include <cstdint>
#include <string_view>

namespace debug
{

extern std::string_view (&uint8_t2string)(uint8_t value);
extern std::string_view (&uint16_t2string)(uint16_t value);
extern std::string_view (&uint32_t2string)(uint32_t value);
extern std::string_view (&uint64_t2string)(uint64_t value);
extern std::string_view (&int8_t2string)(int8_t value);
extern std::string_view (&int16_t2string)(int16_t value);
extern std::string_view (&int32_t2string)(int32_t value);
extern std::string_view (&int64_t2string)(int64_t value);

extern std::string_view (&uint8_t2hexstring)(uint8_t value,   bool uppercaseLetters);
extern std::string_view (&uint16_t2hexstring)(uint16_t value, bool uppercaseLetters);
extern std::string_view (&uint32_t2hexstring)(uint32_t value, bool uppercaseLetters);
extern std::string_view (&uint64_t2hexstring)(uint64_t value, bool uppercaseLetters);
extern std::string_view (&int8_t2hexstring)(int8_t value,     bool uppercaseLetters);
extern std::string_view (&int16_t2hexstring)(int16_t value,   bool uppercaseLetters);
extern std::string_view (&int32_t2hexstring)(int32_t value,   bool uppercaseLetters);
extern std::string_view (&int64_t2hexstring)(int64_t value,   bool uppercaseLetters);

std::string_view float2string(float value);

}
