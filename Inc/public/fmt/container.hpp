#ifndef FMT_CONTAINER_HPP
#define FMT_CONTAINER_HPP

#include "fmt/fmt.h"
#include "fmt/container_type_traits.hpp"

FMT_BEGIN_NAMESPACE

template <typename OStreamT, typename IteratorT>
void StreamElement(OStreamT &stream, IteratorT obj);

template <typename OStreamT, typename IteratorT>
OStreamT & StreamContainer(OStreamT &stream, IteratorT begin, IteratorT end);

template <typename OStreamT, typename IteratorT>
OStreamT & StreamMap(OStreamT &stream, IteratorT begin, IteratorT end);

template <typename OStreamT, size_t idx = 0, typename ...T>
OStreamT & StreamTuple(OStreamT &stream, const std::tuple<T...> &tuple);

FMT_END_NAMESPACE

FMT_ABI::OStream & operator<<(FMT_ABI::OStream &stream, const std::string &str);

template <typename OStreamT, typename T, FMT_ABI::EnableIfT<FMT_ABI::IsContainerV<T>> = true>
OStreamT & operator<<(OStreamT &stream, const T &container);

template <typename OStreamT, typename T, FMT_ABI::EnableIfT<FMT_ABI::IsMapV<T>> = true>
OStreamT & operator<<(OStreamT &stream, const T &map);

template <typename OStreamT, typename ...T>
OStreamT & operator<<(OStreamT &stream, const std::tuple<T...> &tuple);

template <typename OStreamT, typename T1, typename T2>
OStreamT & operator<<(OStreamT &stream, const std::pair<T1, T2> &pair);

#include "fmt/container.inl"

#endif /* FMT_CONTAINER_HPP */