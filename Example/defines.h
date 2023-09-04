#ifndef DEFINES_H
#define DEFINES_H

#include "fmt/stdout.h"

#ifdef DEBUG
# define PRINTF(...)       FmtPrintf(__VA_ARGS__)
# define LOG(STR)          FmtWrite(STR, sizeof(STR) - 1u)
# define ASSERT(EXPR)      (EXPR) ? (void)0 : FmtAssertFailed(#EXPR, __FILE__, __LINE__)

#else
# define PRINTF(...)
# define LOG(STR)
# define ASSERT(EXPR)

#endif /* DEBUG */
#endif /* DEFINES_H */
