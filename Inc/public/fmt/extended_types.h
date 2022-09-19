#pragma once

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>

#ifdef __SIZEOF_INT128__
typedef          __int128 FmtInt128;
typedef unsigned __int128 FmtUInt128;
#define FMT_INT128_SUPPORT

#if defined(__SIZEOF_FLOAT80__) && (LDBL_MANT_DIG != 64)
typedef __float80 FmtFloat80;
#define FMT_FLOAT80_SUPPORT
#endif

#if defined(__SIZEOF_FLOAT128__) && (LDBL_MANT_DIG != 113)
typedef __float128 FmtFloat128;
#define FMT_FLOAT128_SUPPORT
#endif

#endif // __SIZEOF_INT128__

#if defined(FLT16_MANT_DIG) && (FLT16_MANT_DIG > 0)
typedef _Float16 FmtFloat16;
#define FMT_FLOAT16_SUPPORT
#endif
