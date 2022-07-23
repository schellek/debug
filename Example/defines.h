#pragma once

#include "fmt/stdout.h"

#ifdef DEBUG
#define PRINTF(...)       fmt_printf(__VA_ARGS__)
#define LOG(STR)          fmt_write(STR, sizeof(STR) - 1U)
#define ASSERT(EXPR)      (EXPR) ? (void)0 : fmt_assert_failed(#EXPR, __FILE__, __LINE__)

#else
#define PRINTF(...)
#define LOG(STR)
#define ASSERT(EXPR)

#endif /* DEBUG */