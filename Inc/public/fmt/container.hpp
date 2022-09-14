#pragma once

#include "fmt/fmt.h"
#include "fmt/container_type_traits.hpp"

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

template <typename OStream, typename T, std::enable_if_t<FMT_ABI::__IsContainer<T>::value, bool> = true>
OStream & operator<<(OStream &stream, const T &container) noexcept;

template <typename OStream, typename T, std::enable_if_t<FMT_ABI::__IsMap<T>::value, bool> = true>
OStream & operator<<(OStream &stream, const T &map) noexcept;

template <typename OStream, typename ...T>
OStream & operator<<(OStream &stream, const std::tuple<T...> &tuple) noexcept;

template <typename OStream, typename T1, typename T2>
OStream & operator<<(OStream &stream, const std::pair<T1, T2> &pair) noexcept;

#include "fmt/container.inl"
