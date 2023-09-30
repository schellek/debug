#ifndef FMT_EXTENDED_TYPES_H
#define FMT_EXTENDED_TYPES_H

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>

#if !((defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__))
# define __extension__
#endif

#ifdef __SIZEOF_INT128__
__extension__ typedef          __int128 FmtInt128;
__extension__ typedef unsigned __int128 FmtUInt128;
# define FMT_INT128_SUPPORT

# if defined(__SIZEOF_FLOAT80__) && (LDBL_MANT_DIG != 64)
__extension__ typedef __float80 FmtFloat80;
#   define FMT_FLOAT80_SUPPORT
# endif

# if defined(__SIZEOF_FLOAT128__) && (LDBL_MANT_DIG != 113)
__extension__ typedef __float128 FmtFloat128;
#   define FMT_FLOAT128_SUPPORT
# endif

#endif // __SIZEOF_INT128__

#if defined(FLT16_MANT_DIG) && (FLT16_MANT_DIG > 0)
__extension__ typedef _Float16 FmtFloat16;
# define FMT_FLOAT16_SUPPORT
#endif

#endif /* FMT_EXTENDED_TYPES_H */
