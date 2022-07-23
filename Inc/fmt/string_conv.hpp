#pragma once

#include <string_view>
#include <concepts>


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
std::string_view toString(std::integral auto value) noexcept;

/**
 * @brief         Converts a floating point value to a decimal string representation
 * @details       The returned string view points to an internal array whose validity or value may be altered by any
 *                subsequent call to toString, toOctString or toHexString
 * @param[in]     value: Value, that is to be converted
 * @return        std::string_view
 */
std::string_view toString(std::floating_point auto value) noexcept;

/**
 * @brief         Converts an integral value to a hexadecimal string representation
 * @details       The returned string view points to an internal array whose validity or value may be altered by any
 *                subsequent call to toString, toOctString or toHexString
 * @param[in]     value: Value, that is to be converted
 * @param         uppercase: Flag, to convert the hexadecimal digits with upper- or lowercase letters
 * @return        std::string_view
 */
std::string_view toHexString(std::integral auto value, bool uppercase) noexcept;

/**
 * @brief         Converts an integral value to a octal string representation
 * @details       The returned string view points to an internal array whose validity or value may be altered by any
 *                subsequent call to toString, toOctString or toHexString
 * @param[in]     value: Value, that is to be converted
 * @return        std::string_view
 */
std::string_view toOctString(std::integral auto value) noexcept;

} // namespace fmt
