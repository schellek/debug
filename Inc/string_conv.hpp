#pragma once

#include <cstdint>

namespace debug
{

struct string
{
  char *data;
  uint16_t len;
};

extern string (&uint8_t2string)(uint8_t value);
extern string (&uint16_t2string)(uint16_t value);
extern string (&uint32_t2string)(uint32_t value);
extern string (&uint64_t2string)(uint64_t value);
extern string (&int8_t2string)(int8_t value);
extern string (&int16_t2string)(int16_t value);
extern string (&int32_t2string)(int32_t value);
extern string (&int64_t2string)(int64_t value);

extern string (&uint8_t2hexstring)(uint8_t value,   bool uppercaseLetters);
extern string (&uint16_t2hexstring)(uint16_t value, bool uppercaseLetters);
extern string (&uint32_t2hexstring)(uint32_t value, bool uppercaseLetters);
extern string (&uint64_t2hexstring)(uint64_t value, bool uppercaseLetters);
extern string (&int8_t2hexstring)(int8_t value,     bool uppercaseLetters);
extern string (&int16_t2hexstring)(int16_t value,   bool uppercaseLetters);
extern string (&int32_t2hexstring)(int32_t value,   bool uppercaseLetters);
extern string (&int64_t2hexstring)(int64_t value,   bool uppercaseLetters);

string float2string(float value);
}
