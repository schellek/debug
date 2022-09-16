#pragma once

#include <string_view>

#include "fmt/fmt.h"
#include <fmt/type_traits.hpp>

FMT_BEGIN_NAMESPACE

/**
 * @brief         Converts a boolean value to a string representation
 * @param[in]     value: Value, that is to be converted
 * @return        std::string_view
 */
std::string_view toString(bool value) noexcept;

/**
 * @brief         Converts an integral value to a decimal string representation
 * @details       The returned string view points to an internal array whose validity or value may be altered by any
 *                subsequent call to toString, toOctString or toHexString
 * @param[in]     value: Value, that is to be converted
 * @return        std::string_view
 */
template <typename T, EnableIfT<IsIntegralV<T>> = true>
std::string_view toString(T value) noexcept;

/**
 * @brief         Converts a floating point value to a decimal string representation
 * @details       The returned string view points to an internal array whose validity or value may be altered by any
 *                subsequent call to toString, toOctString or toHexString
 * @param[in]     value: Value, that is to be converted
 * @return        std::string_view
 */
template <typename T, EnableIfT<IsFloatingPointV<T>> = true>
std::string_view toString(T value) noexcept;

/**
 * @brief         Converts an integral value to a hexadecimal string representation
 * @details       The returned string view points to an internal array whose validity or value may be altered by any
 *                subsequent call to toString, toOctString or toHexString
 * @param[in]     value: Value, that is to be converted
 * @param[in]     uppercase: Flag, to convert the hexadecimal digits with upper- or lowercase letters
 * @param[in]     prefix: Flag, to convert the value with prefix 0x or 0X
 * @return        std::string_view
 */
template <typename T, EnableIfT<IsIntegralV<T>> = true>
std::string_view toHexString(T value, bool uppercase = false, bool prefix = false) noexcept;

/**
 * @brief         Converts an integral value to a octal string representation
 * @details       The returned string view points to an internal array whose validity or value may be altered by any
 *                subsequent call to toString, toOctString or toHexString
 * @param[in]     value: Value, that is to be converted
 * @param[in]     prefix: Flag, to convert the value with prefix 0
 * @return        std::string_view
 */
template <typename T, EnableIfT<IsIntegralV<T>> = true>
std::string_view toOctString(T value, bool prefix = false) noexcept;

FMT_END_NAMESPACE
