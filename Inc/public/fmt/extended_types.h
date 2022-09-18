#pragma once

#include <float.h>

#ifdef __SIZEOF_INT128__
typedef          __int128 FmtInt128;
typedef unsigned __int128 FmtUInt128;
#define FMT_INT128_SUPPORT

#if defined(__SIZEOF_FLOAT80__) && (LDBL_MANT_DIG != 64)
typedef __float80 FmtFloat80;
#define FMT_FLOAT80_SUPPORT
#endif

#if defined(__SIZEOF_FLOAT128__) && (LDBL_MANT_DIG != 112)
typedef __float128 FmtFloat128;
#define FMT_FLOAT128_SUPPORT
#endif

#endif // __SIZEOF_INT128__

#if 0
typedef __fp16 FmtFloat16;
#define FMT_FLOAT16_SUPPORT

#endif // 0

#if 0
typedef __bf16 FmtBFloat16;
#define FMT_BFLOAT16_SUPPORT

#endif // 0
