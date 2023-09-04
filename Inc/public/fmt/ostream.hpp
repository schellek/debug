#ifndef FMT_OSTREAM_HPP
#define FMT_OSTREAM_HPP

#include <cstdarg>
#include <string_view>

#include "fmt/fmt.h"
#include "fmt/type_traits.hpp"

FMT_BEGIN_NAMESPACE

class OStream
{
public:
  using tSize  = tFmtSize;
  using ManipFunc = OStream & (OStream &);

  struct tWrite
  {
    using StaticFunc = tSize (const char *, tSize);
    using MemberFunc = tSize (void *, const char *, tSize);

    union
    {
      StaticFunc *staticFunc;
      MemberFunc *memberFunc;
    };
    tSize (OStream::*external)(const char *, tSize);

    tWrite(StaticFunc *f);
    tWrite(MemberFunc *f);
  };

  struct tFlush
  {
    using StaticFunc = void (void);
    using MemberFunc = void (void *);

    union
    {
      StaticFunc *staticFunc;
      MemberFunc *memberFunc;
    };
    void (OStream::*external)(void);

    tFlush(StaticFunc *f);
    tFlush(MemberFunc *f);
  };

  OStream() = delete;
  OStream(tWrite::StaticFunc *write, tFlush::StaticFunc *flush = nullptr);
  OStream(void *obj, tWrite::MemberFunc *write, tFlush::MemberFunc *flush = nullptr);

  tSize write(char c);
  tSize write(char c, tSize n);
  tSize write(const char *str, tSize len);
  tSize write(std::string_view str);

  void flush(void);

  int vprintf(const char *str, va_list args);
  int printf(FMT_PRINTF_FMTSTR const char *str, ...) FMT_PRINTF_FUNC(2);

  OStream & operator<<(bool value);
  OStream & operator<<(char value);
  OStream & operator<<(char *str);
  OStream & operator<<(const char *str);
  OStream & operator<<(std::string_view str);
  OStream & operator<<(std::nullptr_t);

  template <typename T, EnableIfT<IsIntegralV<T>> = true>
  OStream & operator<<(T value);

  template <typename T, EnableIfT<IsFloatingPointV<T>> = true>
  OStream & operator<<(T value);

  template <typename T, EnableIfT<IsPointerV<T>> = true>
  OStream & operator<<(T value);

  template <typename T, EnableIfT<IsSmartPointerV<T>> = true>
  OStream & operator<<(T &value);

  OStream & operator<<(const void *p);
  OStream & operator<<(ManipFunc &function);

private:
  tSize _write(const char *str, tSize len);
  tSize staticWrite(const char *str, tSize len);
  tSize memberWrite(const char *str, tSize len);
  tSize dummyWrite(const char *str, tSize len);

  void _flush(void);
  void staticFlush(void);
  void memberFlush(void);
  void dummyFlush(void);

  void *const obj_;
  const tWrite write_;
  const tFlush flush_;
};

OStream & Flush(OStream &stream);
OStream & Endl(OStream &stream);

using ostream = OStream;
static inline constexpr OStream::ManipFunc &flush = Flush;
static inline constexpr OStream::ManipFunc &endl  = Endl;

extern OStream cout;

FMT_END_NAMESPACE

#include "ostream.inl"

#endif /* FMT_OSTREAM_HPP */