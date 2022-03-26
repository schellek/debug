#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void Debug_Write(const char *str, uint16_t len);
void Debug_Printf(const char* str, ...);
void Debug_Puts(const char *str);
void Debug_PutChar(char c);
void Debug_AssertFailed(const char *expr, const char *file, uint32_t line);
void Debug_HAL_AssertFailed(const char *file, uint32_t line);

#ifdef __cplusplus
}
#endif // __cplusplus