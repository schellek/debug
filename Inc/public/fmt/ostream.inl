#ifndef FMT_OSTREAM_INL
#define FMT_OSTREAM_INL

#include "fmt/ostream.hpp"

FMT_BEGIN_NAMESPACE

template <typename T, EnableIfT<IsPointerV<T>>>
OStream & OStream::operator<<(T value)
{
  *this << reinterpret_cast<const void *>(value);
  return *this;
}

template <typename T, EnableIfT<IsSmartPointerV<T>>>
OStream & OStream::operator<<(T &value)
{
  *this << value.get();
  return *this;
}

FMT_END_NAMESPACE

#endif /* FMT_OSTREAM_INL */
