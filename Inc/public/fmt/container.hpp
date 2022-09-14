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

#include "fmt/fmt.h"

FMT_BEGIN_NAMESPACE

template <typename OStream, typename Iterator>
void streamElement(OStream &stream, Iterator obj) noexcept;

template <typename OStream, typename Iterator>
OStream & streamContainer(OStream &stream, Iterator begin, Iterator end) noexcept;

template <typename OStream, typename Iterator>
OStream & streamMap(OStream &stream, Iterator begin, Iterator end) noexcept;

template <typename OStream, size_t idx = 0, typename ...T>
OStream & streamTuple(OStream &stream, const std::tuple<T...> &tuple) noexcept;

FMT_END_NAMESPACE

FMT_ABI::ostream & operator<<(FMT_ABI::ostream &stream, const std::string &str) noexcept;

template <typename OStream, typename T, size_t N>
OStream & operator<<(OStream &stream, const std::array<T, N> &array) noexcept;

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::vector<T, Alloc> &vector) noexcept;

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::deque<T, Alloc> &deque) noexcept;

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::forward_list<T, Alloc> &list) noexcept;

template <typename OStream, typename T, typename Alloc>
OStream & operator<<(OStream &stream, const std::list<T, Alloc> &list) noexcept;

template <typename OStream, typename Key, typename Compare, typename Alloc>
OStream & operator<<(OStream &stream, const std::set<Key, Compare, Alloc> &set) noexcept;

template <typename OStream, typename Key, typename Hash, typename Pred, typename Alloc>
OStream & operator<<(OStream &stream, const std::unordered_set<Key, Hash, Pred, Alloc> &set) noexcept;

template <typename OStream, typename Key, typename Value, typename Compare, typename Alloc>
OStream & operator<<(OStream &stream, const std::map<Key, Value, Compare, Alloc> &map) noexcept;

template <typename OStream, typename Key, typename Value, typename Hash, typename Pred, typename Alloc>
OStream & operator<<(OStream &stream, const std::unordered_map<Key, Value, Hash, Pred, Alloc> &map) noexcept;

template <typename OStream, typename ...T>
OStream & operator<<(OStream &stream, const std::tuple<T...> &tuple) noexcept;

template <typename OStream, typename T1, typename T2>
OStream & operator<<(OStream &stream, const std::pair<T1, T2> &pair) noexcept;

#include "fmt/container.inl"
