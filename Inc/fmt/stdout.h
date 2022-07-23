#pragma once

#include <stdint.h>

#ifndef FMT_ENDL
#define FMT_ENDL "\n"
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef uint16_t ostream_size_type;

/**
 * @brief         Writes a string to stdout
 * @param[in]     str: String to be written
 * @param[in]     len: Length of the string
 * @return        ostream_size_type: Number of characters written
 */
ostream_size_type fmt_write(const char *str, ostream_size_type len);

/**
 * @brief         Prints a formatted string to stdout
 * @param[in]     str: String to be written
 * @param[in/out] ...: TODO
 * @return        int: Number of characters written
 */
int fmt_printf(const char* str, ...);

/**
 * @brief         Writes a string to stdout followed by an end of line
 * @param[in]     str: String to be written
 * @return        uint16_t: Number of characters written
 */
int fmt_puts(const char *str);

/**
 * @brief         Writes a character to stdout
 * @param[in]     c: Character to be written
 * @return        int: Number of characters written
 */
int fmt_putchar(int c);

/**
 * @brief 
 * @param expr 
 * @param file 
 * @param line 
 */
void fmt_assert_failed(const char *expr, const char *file, uint32_t line);

#ifdef __cplusplus
}
#endif // __cplusplus
