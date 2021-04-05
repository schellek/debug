#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void Debug_Write(const char *p_str, uint16_t len);
void Debug_Printf(const char* p_str, ...);
void Debug_Puts(const char *p_str);
void Debug_PutChar(char c);
void Debug_AssertFailed(const char *sz_expr, const char *sz_file, uint32_t line);
void Debug_HAL_AssertFailed(const char *sz_file, uint32_t line);

#ifdef __cplusplus
}
#endif // __cplusplus