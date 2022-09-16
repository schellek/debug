#pragma once

#ifdef _LIBCPP_VERSION
#ifndef _LIBCPP_HAS_NO_INT128
typedef __int128_t  fmt_int128_t;
typedef __uint128_t fmt_uint128_t;
#define FMT_INT128_SUPPORT

#endif // _LIBCPP_HAS_NO_INT128

#endif // _LIBCPP_VERSION

#if defined(FMT_INT128_SUPPORT) && 0
typedef __float128 fmt_float128_t;
#define FMT_FLOAT128_SUPPORT

#endif // 0

#if 0
typedef __fp16 fmt_float16_t;
#define FMT_FLOAT16_SUPPORT

#endif // 0

#if 0
typedef __bf16 fmt_bfloat16_t;
#define FMT_BFLOAT16_SUPPORT

#endif // 0
