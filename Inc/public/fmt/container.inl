#pragma once

#include "fmt/fmt.h"
#include "fmt/container.hpp"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

template <typename OStreamT, typename IteratorT>
void StreamElement(OStreamT &stream, IteratorT element) noexcept
{
  using T = RemoveCVRefT<decltype(*element)>;

  if constexpr (IsStringTypeV<T>)
    stream << '\"' << *element << '\"';
  else if constexpr (IsSameV<T, char>)
    stream << '\'' << *element << '\'';
  else
    stream << *element;
}

template <typename OStreamT, typename IteratorT>
OStreamT & StreamContainer(OStreamT &stream, IteratorT begin, IteratorT end) noexcept
{
  IteratorT it = begin;

  stream << '[';

  if (it != end)
    StreamElement(stream, it++);

  for (; it != end; ++it)
  {
    stream << ", ";
    StreamElement(stream, it);
  }

  stream << ']';

  return stream;
}

template <typename OStreamT, typename IteratorT>
OStreamT & StreamMap(OStreamT &stream, IteratorT begin, IteratorT end) noexcept
{
  IteratorT it = begin;

  stream << '{';

  if (it != end)
  {
    const auto & [key, value] = *(it++);
    StreamElement(stream, &key);
    stream << ": ";
    StreamElement(stream, &value);
  }

  for (; it != end; ++it)
  {
    const auto & [key, value] = *it;
    stream << ", ";
    StreamElement(stream, &key);
    stream << ": ";
    StreamElement(stream, &value);
  }

  stream << '}';

  return stream;
}

template <typename OStreamT, size_t idx, typename ...T>
OStreamT & StreamTuple(OStreamT &stream, const std::tuple<T...> &tuple) noexcept
{
  if constexpr (idx < sizeof...(T))
  {
    if constexpr (idx == 0U)
      stream << '(';
    else
      stream << ", ";

    StreamElement(stream, &std::get<idx>(tuple));
    StreamTuple<OStreamT, idx + 1, T...>(stream, tuple);
  }
  else
  {
    stream << ')';
  }

  return stream;
}

FMT_END_NAMESPACE

FMT_ABI::OStream & operator<<(FMT_ABI::OStream &stream, const std::string &str) noexcept
{
  stream.write(str.data(), static_cast<FMT_ABI::OStream::SizeType>(str.size()));
  return stream;
}

template <typename OStreamT, typename T, FMT_ABI::EnableIfT<FMT_ABI::IsContainerV<T>>>
OStreamT & operator<<(OStreamT &stream, const T &container) noexcept
{
  return FMT_ABI::StreamContainer(stream, container.cbegin(), container.cend());
}

template <typename OStreamT, typename T, FMT_ABI::EnableIfT<FMT_ABI::IsMapV<T>>>
OStreamT & operator<<(OStreamT &stream, const T &map) noexcept
{
  return FMT_ABI::StreamMap(stream, map.cbegin(), map.cend());
}

template <typename OStreamT, typename ...T>
OStreamT & operator<<(OStreamT &stream, const std::tuple<T...> &tuple) noexcept
{
  return FMT_ABI::StreamTuple(stream, tuple);
}

template <typename OStreamT, typename T1, typename T2>
OStreamT & operator<<(OStreamT &stream, const std::pair<T1, T2> &pair) noexcept
{
  return FMT_ABI::StreamTuple(stream, std::make_tuple(std::ref(pair.first), std::ref(pair.second)));
}
