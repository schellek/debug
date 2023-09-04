#ifndef FMT_FMT_H
#define FMT_FMT_H

#include <stdint.h>

#ifndef FMT_ENDL
# define FMT_ENDL "\n"
#endif

#define FMT_ABI Fmt::v1
#define FMT_BEGIN_NAMESPACE namespace Fmt { inline namespace v1 {
#define FMT_END_NAMESPACE } }

#ifdef __cplusplus
# define FMT_BEGIN_GLOBAL_NAMESPACE extern "C" {
# define FMT_END_GLOBAL_NAMESPACE }
#else
# define FMT_BEGIN_GLOBAL_NAMESPACE
# define FMT_END_GLOBAL_NAMESPACE
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# define FMT_PRINTF_FMTSTR         _Printf_format_string_
# define FMT_PRINTF_FUNC(FMT_ARG)

#elif defined(__GNUC__)
# define FMT_PRINTF_FMTSTR
# define FMT_PRINTF_FUNC(FMT_ARG)  __attribute__((format(__printf__, FMT_ARG, FMT_ARG + 1)))

#else
# define FMT_PRINTF_FMTSTR
# define FMT_PRINTF_FUNC(FMT_ARG)

#endif

typedef uint16_t tFmtSize;

#endif /* FMT_FMT_H */
