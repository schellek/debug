#pragma once

#include "fmt/stdout.h"

#ifdef DEBUG
#define PRINTF(...)       fmt_printf(__VA_ARGS__)
#define LOG(__STR__)      fmt_write(__STR__, sizeof(__STR__) - 1U)
#define ASSERT(__EXPR__)  (__EXPR__) ? (void)0 : fmt_assert_failed(#__EXPR__, __FILE__, __LINE__)

#else
#define PRINTF(...)
#define LOG(__STR__)
#define ASSERT(__EXPR__)

#endif /* DEBUG */