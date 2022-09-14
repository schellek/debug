#pragma once

#include <type_traits>
#include <string_view>
#include <string>
#include <memory>
#include "fmt/fmt.h"

FMT_BEGIN_NAMESPACE

template <typename U, typename ...V>
struct IsAnyOf : std::disjunction<std::is_same<U, V>...> {};

template <typename U, typename ...V>
inline constexpr bool IsAnyOf_v = IsAnyOf<U, V...>::value;

template <typename T>
using RemoveCV = std::remove_cv<T>;

template <typename T>
using RemoveCV_t = typename RemoveCV<T>::type;

template <typename T>
using AddCV = std::add_cv<T>;

template <typename T>
using AddCV_t = typename AddCV<T>::type;

template <typename T>
using RemoveRef = std::remove_reference<T>;

template <typename T>
using RemoveRef_t = typename RemoveRef<T>::type;

template <typename T>
using AddRef = std::add_lvalue_reference<T>;

template <typename T>
using AddRef_t = typename AddRef<T>::type;

template <typename T>
struct RemoveCVRef : RemoveCV<RemoveRef_t<T>> {};

template <typename T>
using RemoveCVRef_t = typename RemoveCVRef<T>::type;

template <typename T>
struct AddCVRef : AddRef<AddCV_t<T>> {};

template <typename T>
using AddCVRef_t = typename AddCVRef<T>::type;

template <typename T>
using RemovePtr = std::remove_pointer<T>;

template <typename T>
using RemovePtr_t = typename RemovePtr<T>::type;

template <typename T>
using AddPtr = std::add_pointer<T>;

template <typename T>
using AddPtr_t = typename AddPtr<T>::type;

template <typename T, bool = std::is_pointer<T>::value>
struct __RemoveCVFromPtr { using type = T; };

template <typename T>
struct __RemoveCVFromPtr<T, true> { using type = AddPtr_t<RemoveCV_t<RemovePtr_t<T>>>; };

template <typename T>
struct RemoveCVFromPtr : __RemoveCVFromPtr<T> {};

template <typename T>
using RemoveCVFromPtr_t = typename RemoveCVFromPtr<T>::type;

template <typename T, typename D = std::default_delete<T>>
struct __IsSmartPtr : std::false_type {};

template <typename T, typename D>
struct __IsSmartPtr<std::unique_ptr<T, D>> : std::true_type {};

template <typename T>
struct __IsSmartPtr<std::shared_ptr<T>> : std::true_type {};

template <typename T>
struct __IsSmartPtr<std::weak_ptr<T>> : std::true_type {};

template <typename T>
struct IsSmartPtr : __IsSmartPtr<RemoveCVRef_t<T>> {};

template <typename T>
struct IsCharType : public std::is_same<RemoveCVRef_t<T>, char> {};

template <typename T>
struct __IsStringType : std::false_type {};

template <>
struct __IsStringType<char *> : std::true_type {};

template <>
struct __IsStringType<std::string> : std::true_type {};

template <>
struct __IsStringType<std::string_view> : std::true_type {};

template <typename T>
struct IsStringType : __IsStringType<RemoveCVFromPtr_t<RemoveCVRef_t<T>>> {};

FMT_END_NAMESPACE
