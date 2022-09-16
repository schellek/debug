#pragma once

#include "fmt/fmt.h"
#include "fmt/container.hpp"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

template <typename OStream, typename Iterator>
void streamElement(OStream &stream, Iterator element) noexcept
{
  using T = RemoveCVRefT<decltype(*element)>;

  if constexpr (IsStringTypeV<T>)
    stream << '\"' << *element << '\"';
  else if constexpr (IsSameV<T, char>)
    stream << '\'' << *element << '\'';
  else
    stream << *element;
}

template <typename OStream, typename Iterator>
OStream & streamContainer(OStream &stream, Iterator begin, Iterator end) noexcept
{
  Iterator it = begin;

  stream << '[';

  if (it != end)
    streamElement(stream, it++);

  for (; it != end; ++it)
  {
    stream << ", ";
    streamElement(stream, it);
  }

  stream << ']';

  return stream;
}

template <typename OStream, typename Iterator>
OStream & streamMap(OStream &stream, Iterator begin, Iterator end) noexcept
{
  Iterator it = begin;

  stream << '{';

  if (it != end)
  {
    const auto & [key, value] = *(it++);
    streamElement(stream, &key);
    stream << ": ";
    streamElement(stream, &value);
  }

  for (; it != end; ++it)
  {
    const auto & [key, value] = *it;
    stream << ", ";
    streamElement(stream, &key);
    stream << ": ";
    streamElement(stream, &value);
  }

  stream << '}';

  return stream;
}

template <typename OStream, size_t idx, typename ...T>
OStream & streamTuple(OStream &stream, const std::tuple<T...> &tuple) noexcept
{
  if constexpr (idx < sizeof...(T))
  {
    if constexpr (idx == 0U)
      stream << '(';
    else
      stream << ", ";

    streamElement(stream, &std::get<idx>(tuple));
    streamTuple<OStream, idx + 1, T...>(stream, tuple);
  }
  else
  {
    stream << ')';
  }

  return stream;
}

FMT_END_NAMESPACE

FMT_ABI::ostream & operator<<(FMT_ABI::ostream &stream, const std::string &str) noexcept
{
  stream.write(str.data(), static_cast<FMT_ABI::ostream::size_type>(str.size()));
  return stream;
}

template <typename OStream, typename T, FMT_ABI::EnableIfT<FMT_ABI::IsContainerV<T>>>
OStream & operator<<(OStream &stream, const T &container) noexcept
{
  return FMT_ABI::streamContainer(stream, container.cbegin(), container.cend());
}

template <typename OStream, typename T, FMT_ABI::EnableIfT<FMT_ABI::IsMapV<T>>>
OStream & operator<<(OStream &stream, const T &map) noexcept
{
  return FMT_ABI::streamMap(stream, map.cbegin(), map.cend());
}

template <typename OStream, typename ...T>
OStream & operator<<(OStream &stream, const std::tuple<T...> &tuple) noexcept
{
  return FMT_ABI::streamTuple(stream, tuple);
}

template <typename OStream, typename T1, typename T2>
OStream & operator<<(OStream &stream, const std::pair<T1, T2> &pair) noexcept
{
  return FMT_ABI::streamTuple(stream, std::make_tuple(std::ref(pair.first), std::ref(pair.second)));
}
