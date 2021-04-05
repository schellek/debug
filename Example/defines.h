#include "debugio.h"

#ifdef DEBUG
#define DEBUG_PRINTF(...)   Debug_Printf(__VA_ARGS__)
#define DEBUG_LOG(__STR__)  Debug_Write(__STR__, sizeof(__STR__) - 1U)
#define ASSERT(__EXPR__)    (__EXPR__) ? (void)0 : Debug_AssertFailed(#__EXPR__, __FILE__, __LINE__)
#else
#define DEBUG_PRINTF(...)
#define DEBUG_LOG(__STR__)
#define ASSERT(__EXPR__)
#endif