#pragma once

#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <tuple>
#include <string>
#include "fmt/ostream.hpp"


namespace fmt
{

template <typename OStream, typename Iterator>
OStream & streamContainer(OStream &stream, Iterator begin, Iterator end) noexcept
{
  Iterator it = begin;

  stream << '[';

  if (it != end)
    stream << *(it++);

  for (; it != end; ++it)
    stream << ", " << *it;

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
    stream << key << ": " << value;
  }

  for (; it != end; ++it)
  {
    const auto & [key, value] = *it;
    stream << ", " << key << ": " << value;
  }

  stream << '}';

  return stream;
}

template <typename OStream, size_t idx = 0, typename ...T>
OStream & streamTuple(OStream &stream, const std::tuple<T...> &tuple) noexcept
{
  if constexpr (idx < sizeof...(T))
  {
    if constexpr (idx == 0U)
      stream << '(';
    else
      stream << ", ";

    stream << std::get<idx>(tuple);
    streamTuple<OStream, idx + 1, T...>(stream, tuple);
  }
  else
  {
    stream << ')';
  }

  return stream;
}

} // namespace fmt

template <typename OStream, typename T, size_t N>
OStream & operator<<(OStream &stream, const std::array<T, N> &array) noexcept
{
  return fmt::streamContainer(stream, array.cbegin(), array.cend());
}

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::vector<T, Alloc> &vector) noexcept
{
  return fmt::streamContainer(stream, vector.cbegin(), vector.cend());
}

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::deque<T, Alloc> &deque) noexcept
{
  return fmt::streamContainer(stream, deque.cbegin(), deque.cend());
}

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::forward_list<T, Alloc> &list) noexcept
{
  return fmt::streamContainer(stream, list.cbegin(), list.cend());
}

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::list<T, Alloc> &list) noexcept
{
  return fmt::streamContainer(stream, list.cbegin(), list.cend());
}

template <typename OStream, typename Key, typename Compare, typename Alloc>
OStream & operator<<(OStream &stream, const std::set<Key, Compare, Alloc> &set) noexcept
{
  return fmt::streamContainer(stream, set.cbegin(), set.cend());
}

template <typename OStream, typename Key, typename Hash, typename Pred, typename Alloc>
OStream & operator<<(OStream &stream, const std::unordered_set<Key, Hash, Pred, Alloc> &set) noexcept
{
  return fmt::streamContainer(stream, set.cbegin(), set.cend());
}

template <typename OStream, typename Key, typename Value, typename Compare, typename Alloc>
OStream & operator<<(OStream &stream, const std::map<Key, Value, Compare, Alloc> &map) noexcept
{
  return fmt::streamMap(stream, map.cbegin(), map.cend());
}

template <typename OStream, typename Key, typename Value, typename Hash, typename Pred, typename Alloc>
OStream & operator<<(OStream &stream, const std::unordered_map<Key, Value, Hash, Pred, Alloc> &map) noexcept
{
  return fmt::streamMap(stream, map.cbegin(), map.cend());
}

template <typename OStream, typename ...T>
OStream & operator<<(OStream &stream, const std::tuple<T...> &tuple) noexcept
{
  return fmt::streamTuple(stream, tuple);
}

template <typename OStream, typename T1, typename T2>
OStream & operator<<(OStream &stream, const std::pair<T1, T2> &pair) noexcept
{
  stream << '(' << pair.first << ", " << pair.second << ')';
  return stream;
}

fmt::ostream & operator<<(fmt::ostream &stream, const std::string &str) noexcept
{
  stream.write(str.data(), str.size());
  return stream;
}
