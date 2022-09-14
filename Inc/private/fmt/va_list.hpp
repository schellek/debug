#pragma once

#include <cstdarg>
#include <type_traits>

#include "fmt/fmt.h"

FMT_BEGIN_NAMESPACE

class VaList
{
private:
  va_list m_list;

public:
  VaList(va_list list) noexcept;
  ~VaList() noexcept;

  template <typename T>
  T getArg(void) noexcept;
};

inline VaList::VaList(va_list list) noexcept
{
  va_copy(m_list, list);
}

inline VaList::~VaList() noexcept
{
  va_end(m_list);
}

template <typename T>
inline T VaList::getArg(void) noexcept
{
  static_assert(std::is_arithmetic_v<std::remove_pointer_t<T>> || std::is_same_v<T, void *>);

  using pass_t =  std::conditional_t<std::is_integral_v<T> && (sizeof(T) < sizeof(int)),
                    int,
                    std::conditional_t<std::is_same_v<T, float>,
                      double,
                      T>>;

  return static_cast<T>(va_arg(m_list, pass_t));
}

FMT_END_NAMESPACE
