#pragma once

#include "fmt/fmt.h"

FMT_BEGIN_GLOBAL_NAMESPACE

/**
 * @brief         Writes a string to stdout
 * @param[in]     str: String to be written
 * @param[in]     len: Length of the string
 * @return        fmt_size_type: Number of characters written
 */
fmt_size_type fmt_write(const char *str, fmt_size_type len);

/**
 * @brief         Prints a formatted string to stdout
 * @param[in]     str: String that contains the text to be written to stdout. It can optionally contain embedded
 *                format specifiers that are replaced by the values specified in subsequent additional arguments and
 *                formatted as requested.
 * @param[in/out] ...: Depending on the format string, the function may expect a sequence of additional arguments, each
 *                containing a value to be used to replace a format specifier in the format string (or a pointer to a
 *                storage location, for n).
 * @return        int: Number of characters written
 */
int fmt_printf(FMT_PRINTF_FMTSTR const char *str, ...) FMT_PRINTF_FUNC(1);

/**
 * @brief         Stores a formatted string to a specified buffer
 * @param[out]    buf: Buffer to store the formatted string
 * @param[in]     str: String that contains the text to be written to the buffer. It can optionally contain embedded
 *                format specifiers that are replaced by the values specified in subsequent additional arguments and
 *                formatted as requested.
 * @param[in/out] ...: Depending on the format string, the function may expect a sequence of additional arguments, each
 *                containing a value to be used to replace a format specifier in the format string (or a pointer to a
 *                storage location, for n).
 * @return        int: Number of characters written
 */
int fmt_sprintf(char *buf, FMT_PRINTF_FMTSTR const char *str, ...) FMT_PRINTF_FUNC(2);

/**
 * @brief         Stores a formatted string to a specified buffer with a fixed length
 * @param[out]    buf: Buffer to store the formatted string
 * @param[in]     n: Maximum number of bytes to be used in the buffer
 * @param[in]     str: String that contains the text to be written to the buffer. It can optionally contain embedded
 *                format specifiers that are replaced by the values specified in subsequent additional arguments and
 *                formatted as requested.
 * @param[in/out] ...: Depending on the format string, the function may expect a sequence of additional arguments, each
 *                containing a value to be used to replace a format specifier in the format string (or a pointer to a
 *                storage location, for n).
 * @return        int: Number of characters written
 */
int fmt_snprintf(char *buf, size_t n, FMT_PRINTF_FMTSTR const char *str, ...) FMT_PRINTF_FUNC(3);

/**
 * @brief         Writes a string to stdout followed by an end of line
 * @param[in]     str: String to be written
 * @return        int: Number of characters written
 */
int fmt_puts(const char *str);

/**
 * @brief         Writes a character to stdout
 * @param[in]     c: Character to be written
 * @return        int: Number of characters written
 */
int fmt_putchar(int c);

/**
 * @brief         Flushes stdout
 * @param[]       None
 * @return        None
 */
void fmt_flush(void);

/**
 * @brief
 * @param expr
 * @param file
 * @param line
 */
void fmt_assert_failed(const char *expr, const char *file, uint32_t line);

FMT_END_GLOBAL_NAMESPACE
