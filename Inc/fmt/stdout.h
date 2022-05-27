#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void fmt_write(const char *str, uint16_t len);
void fmt_printf(const char* str, ...);
void fmt_puts(const char *str);
void fmt_putchar(char c);
void fmt_assert_failed(const char *expr, const char *file, uint32_t line);

#ifdef __cplusplus
}
#endif // __cplusplus
