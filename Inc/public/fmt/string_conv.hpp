#pragma once

#include <string_view>
#include <type_traits>

namespace fmt
{

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
template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool> = true>
std::string_view toString(int_t value) noexcept;

/**
 * @brief         Converts a floating point value to a decimal string representation
 * @details       The returned string view points to an internal array whose validity or value may be altered by any
 *                subsequent call to toString, toOctString or toHexString
 * @param[in]     value: Value, that is to be converted
 * @return        std::string_view
 */
template <typename float_t, std::enable_if_t<std::is_floating_point_v<float_t>, bool> = true>
std::string_view toString(float_t value) noexcept;

/**
 * @brief         Converts an integral value to a hexadecimal string representation
 * @details       The returned string view points to an internal array whose validity or value may be altered by any
 *                subsequent call to toString, toOctString or toHexString
 * @param[in]     value: Value, that is to be converted
 * @param[in]     uppercase: Flag, to convert the hexadecimal digits with upper- or lowercase letters
 * @param[in]     prefix: Flag, to convert the value with prefix 0x or 0X
 * @return        std::string_view
 */
template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool> = true>
std::string_view toHexString(int_t value, bool uppercase, bool prefix) noexcept;

/**
 * @brief         Converts an integral value to a octal string representation
 * @details       The returned string view points to an internal array whose validity or value may be altered by any
 *                subsequent call to toString, toOctString or toHexString
 * @param[in]     value: Value, that is to be converted
 * @param[in]     prefix: Flag, to convert the value with prefix 0
 * @return        std::string_view
 */
template <typename int_t, std::enable_if_t<std::is_integral_v<int_t>, bool> = true>
std::string_view toOctString(int_t value, bool prefix) noexcept;

} // namespace fmt
