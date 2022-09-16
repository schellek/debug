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
struct __IsContainer : FalseType {};

template <typename T, size_t N>
struct __IsContainer<std::array<T, N>> : TrueType {};

template <typename ...T>
struct __IsContainer<std::vector<T...>> : TrueType {};

template <typename ...T>
struct __IsContainer<std::deque<T...>> : TrueType {};

template <typename ...T>
struct __IsContainer<std::forward_list<T...>> : TrueType {};

template <typename ...T>
struct __IsContainer<std::set<T...>> : TrueType {};

template <typename ...T>
struct __IsContainer<std::unordered_set<T...>> : TrueType {};

template <typename T>
using IsContainer = __IsContainer<RemoveConstRefT<T>>;

template <typename T>
inline constexpr bool IsContainerV = IsContainer<T>::value;

template <typename T>
struct __IsMap : FalseType {};

template <typename ...T>
struct __IsMap<std::map<T...>> : TrueType {};

template <typename ...T>
struct __IsMap<std::unordered_map<T...>> : TrueType {};

template <typename T>
using IsMap = __IsMap<RemoveConstRefT<T>>;

template <typename T>
inline constexpr bool IsMapV = IsMap<T>::value;

FMT_END_NAMESPACE
