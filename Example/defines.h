#pragma once

#include "fmt/stdout.h"

#ifdef DEBUG
#define PRINTF(...)       FmtPrintf(__VA_ARGS__)
#define LOG(STR)          FmtWrite(STR, sizeof(STR) - 1U)
#define ASSERT(EXPR)      (EXPR) ? (void)0 : FmtAssertFailed(#EXPR, __FILE__, __LINE__)

#else
#define PRINTF(...)
#define LOG(STR)
#define ASSERT(EXPR)

#endif /* DEBUG */