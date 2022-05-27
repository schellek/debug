#pragma once

#include <cstdint>
#include <cstddef>

#include "fmt/ostream.hpp"
#include "fmt/string_conv.hpp"

#ifdef _WIN32
#include <windows.h>
#define WINDOWS_GET_STDOUT_HANDLE()     HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
#define SET_TEXT_COLOR_RED()            SetConsoleTextAttribute(hstdout, 12U)
#define RESET_TEXT_COLOR()              SetConsoleTextAttribute(hstdout, 7U)
#else
#define WINDOWS_GET_STDOUT_HANDLE()
#define SET_TEXT_COLOR_RED()            stream << "\x1B[31m"
#define RESET_TEXT_COLOR()              stream << "\x1B[0m"
#endif

namespace fmt
{

namespace memory
{

template <typename T>
class bin
{
private:
  const uint8_t *m_start;
  constexpr size_t size(void) { return sizeof(T); }

public:
  bin(const T &obj) : m_start(reinterpret_cast<const uint8_t *>(&obj)) {}
  ~bin() { m_start = nullptr; }

  friend ostream &operator<<(ostream &stream, bin obj)
  {
    static constexpr uint8_t rowBitmask = 0x07U;

    const void *lastObjByte = obj.m_start + obj.size() - 1U;
    const uint8_t *memoryStart =
        reinterpret_cast<const uint8_t *>(reinterpret_cast<uintptr_t>(obj.m_start) & ~rowBitmask);
    const uint8_t *memoryStop =
        reinterpret_cast<const uint8_t *>((reinterpret_cast<uintptr_t>(lastObjByte) | rowBitmask) + 1U);
    const uint8_t *currentAddress;
    uint8_t msk;
    bool currentAddressPointsToObj = memoryStart == obj.m_start;
    WINDOWS_GET_STDOUT_HANDLE();

    for (currentAddress = memoryStart; currentAddress < memoryStop; currentAddress++)
    {
      if (reinterpret_cast<uintptr_t>(currentAddress) & rowBitmask)
      {
        /* Do nothing*/
      }
      else if (currentAddressPointsToObj)
      {
        RESET_TEXT_COLOR();
        stream << reinterpret_cast<const void *>(currentAddress) << ": ";
        SET_TEXT_COLOR_RED();
      }
      else
      {
        stream << reinterpret_cast<const void *>(currentAddress) << ": ";
      }

      if (currentAddress == obj.m_start)
      {
        SET_TEXT_COLOR_RED();
        currentAddressPointsToObj = true;
      }

      for (msk = (1U << 7U); msk != 0U; msk >>= 1U)
        stream << ((*currentAddress & msk) ? '1' : '0');

      if (currentAddress == lastObjByte)
      {
        RESET_TEXT_COLOR();
        currentAddressPointsToObj = false;
      }

      stream << (((reinterpret_cast<uintptr_t>(currentAddress) & rowBitmask) == rowBitmask) ? '\n' : ' ');
    }
    return stream;
  }
};

template<typename T>
class hex
{
private:
  const uint8_t *m_start;

public:
  hex(const T& obj) : m_start(reinterpret_cast<const uint8_t *>(&obj)) { }
  ~hex() { m_start = nullptr; }
  constexpr size_t size(void) { return sizeof(T); }


  friend ostream &operator<<(ostream &stream, hex obj)
  {
    static constexpr char lowercaseHexLookup[16] =
      { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    static constexpr uint8_t cellBitmask = 0x3U;
    static constexpr uint8_t rowBitmask = 0x1FU;

    const uint8_t *lastObjByte = obj.m_start + obj.size() - 1U;
    const uint8_t *memoryStart =
        reinterpret_cast<const uint8_t *>(reinterpret_cast<uintptr_t>(obj.m_start) & ~rowBitmask);
    const uint8_t *memoryStop =
        reinterpret_cast<const uint8_t *>((reinterpret_cast<uintptr_t>(lastObjByte) | rowBitmask) + 1U);
    const uint8_t *currentAddress;
    uint8_t msk;
    bool currentAddressPointsToObj = memoryStart == obj.m_start;
    WINDOWS_GET_STDOUT_HANDLE();

    for (currentAddress = memoryStart; currentAddress < memoryStop; currentAddress++)
    {
      if (reinterpret_cast<uintptr_t>(currentAddress) & rowBitmask)
      {
        /* Do nothing*/
      }
      else if (currentAddressPointsToObj)
      {
        RESET_TEXT_COLOR();
        stream << reinterpret_cast<const void *>(currentAddress) << ": ";
        SET_TEXT_COLOR_RED();
      }
      else
      {
        stream << reinterpret_cast<const void *>(currentAddress) << ": ";
      }

      if (currentAddress == obj.m_start)
      {
        SET_TEXT_COLOR_RED();
        currentAddressPointsToObj = true;
      }

      stream << lowercaseHexLookup[*currentAddress >> 4U] << lowercaseHexLookup[*currentAddress & 0x0F];

      if (currentAddress == lastObjByte)
      {
        RESET_TEXT_COLOR();
        currentAddressPointsToObj = false;
      }

      if ((reinterpret_cast<uintptr_t>(currentAddress) & rowBitmask) == rowBitmask)
        stream << '\n';
      else if ((reinterpret_cast<uintptr_t>(currentAddress) & cellBitmask) == cellBitmask)
        stream << ' ';
    }

    return stream;
  }
};

} // namespace memory

} // namespace fmt