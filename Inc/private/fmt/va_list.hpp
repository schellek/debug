#ifndef FMT_VA_LIST_HPP
#define FMT_VA_LIST_HPP

#include <cstdarg>

#include "fmt/fmt.h"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

class VaList
{
private:
  va_list m_list;

public:
  VaList(va_list list);
  ~VaList();

  template <typename T, EnableIfT<IsIntegralV<T>> = true>
  T getArg(void);

  template <typename T, EnableIfT<IsFloatingPointV<T>> = true>
  T getArg(void);

  template <typename T, EnableIfT<IsPointerV<T>> = true>
  T getArg(void);
};

inline VaList::VaList(va_list list)
{
  va_copy(m_list, list);
}

inline VaList::~VaList()
{
  va_end(m_list);
}

template <typename T, EnableIfT<IsIntegralV<T>>>
inline T VaList::getArg(void)
{
  using PassType = ConditionalT<sizeof(T) < sizeof(int), int, T>;
  return static_cast<T>(va_arg(m_list, PassType));
}

template <typename T, EnableIfT<IsFloatingPointV<T>>>
inline T VaList::getArg(void)
{
  using PassType = ConditionalT<sizeof(T) < sizeof(double), double, T>;
  return static_cast<T>(va_arg(m_list, PassType));
}

template <typename T, EnableIfT<IsPointerV<T>>>
inline T VaList::getArg(void)
{
  return va_arg(m_list, T);
}

FMT_END_NAMESPACE

#endif /* FMT_VA_LIST_HPP */