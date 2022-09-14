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

#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

template <typename T>
struct __IsContainer : std::false_type {};

template <typename T, size_t N>
struct __IsContainer<std::array<T, N>> : std::true_type {};

template <typename ...T>
struct __IsContainer<std::vector<T...>> : std::true_type {};

template <typename ...T>
struct __IsContainer<std::deque<T...>> : std::true_type {};

template <typename ...T>
struct __IsContainer<std::forward_list<T...>> : std::true_type {};

template <typename ...T>
struct __IsContainer<std::set<T...>> : std::true_type {};

template <typename ...T>
struct __IsContainer<std::unordered_set<T...>> : std::true_type {};

template <typename T>
struct IsContainer : __IsContainer<RemoveConstRef_t<T>> {};

template <typename T>
struct __IsMap : std::false_type {};

template <typename ...T>
struct __IsMap<std::map<T...>> : std::true_type{};

template <typename ...T>
struct __IsMap<std::unordered_map<T...>> : std::true_type{};

template <typename T>
struct IsMap : __IsMap<RemoveConstRef_t<T>> {};

FMT_END_NAMESPACE
