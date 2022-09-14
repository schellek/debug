#pragma once

#include "fmt/fmt.h"
#include "fmt/container.hpp"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

template <typename OStream, typename Iterator>
void streamElement(OStream &stream, Iterator element) noexcept
{
  using T = decltype(*element);

  if constexpr (IsStringType<T>::value)
    stream << '\"' << *element << '\"';
  else if constexpr (IsCharType<T>::value)
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

template <typename OStream, typename T, size_t N>
OStream & operator<<(OStream &stream, const std::array<T, N> &array) noexcept
{
  return FMT_ABI::streamContainer(stream, array.cbegin(), array.cend());
}

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::vector<T, Alloc> &vector) noexcept
{
  return FMT_ABI::streamContainer(stream, vector.cbegin(), vector.cend());
}

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::deque<T, Alloc> &deque) noexcept
{
  return FMT_ABI::streamContainer(stream, deque.cbegin(), deque.cend());
}

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::forward_list<T, Alloc> &list) noexcept
{
  return FMT_ABI::streamContainer(stream, list.cbegin(), list.cend());
}

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::list<T, Alloc> &list) noexcept
{
  return FMT_ABI::streamContainer(stream, list.cbegin(), list.cend());
}

template <typename OStream, typename Key, typename Compare, typename Alloc>
OStream & operator<<(OStream &stream, const std::set<Key, Compare, Alloc> &set) noexcept
{
  return FMT_ABI::streamContainer(stream, set.cbegin(), set.cend());
}

template <typename OStream, typename Key, typename Hash, typename Pred, typename Alloc>
OStream & operator<<(OStream &stream, const std::unordered_set<Key, Hash, Pred, Alloc> &set) noexcept
{
  return FMT_ABI::streamContainer(stream, set.cbegin(), set.cend());
}

template <typename OStream, typename Key, typename Value, typename Compare, typename Alloc>
OStream & operator<<(OStream &stream, const std::map<Key, Value, Compare, Alloc> &map) noexcept
{
  return FMT_ABI::streamMap(stream, map.cbegin(), map.cend());
}

template <typename OStream, typename Key, typename Value, typename Hash, typename Pred, typename Alloc>
OStream & operator<<(OStream &stream, const std::unordered_map<Key, Value, Hash, Pred, Alloc> &map) noexcept
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
