#pragma once

#include <string_view>
#include <string>
#include <memory>

#include "fmt/fmt.h"
#include "fmt/extended_types.h"

FMT_BEGIN_NAMESPACE

template <bool>
struct EnableIf {};

template <>
struct EnableIf<true> { using type = bool; };

template <bool B>
using EnableIfT = typename EnableIf<B>::type;

template <typename T, T Value>
struct IntegralConstant
{
  using value_type = T;
  static constexpr const value_type value = Value;
};

template <bool Value>
struct BoolConstant : IntegralConstant<bool, Value> {};

using FalseType = BoolConstant<false>;

using TrueType = BoolConstant<true>;

template <typename T1, typename T2>
struct IsSame : FalseType {};

template <typename T>
struct IsSame<T, T> : TrueType {};

template <typename T1, typename T2>
inline constexpr bool IsSameV = IsSame<T1, T2>::value;

template <bool B, typename TrueT, typename FalseT>
struct Conditional { using type = FalseT; };

template <typename TrueT, typename FalseT>
struct Conditional<true, TrueT, FalseT> { using type = TrueT; };

template <bool B, typename TrueT, typename FalseT>
using ConditionalT = typename Conditional<B, TrueT, FalseT>::type;

template <typename T>
using Not = BoolConstant<!T::value>;

template <typename T>
inline constexpr bool NotV = Not<T>::value;

template <typename ...T>
struct And : TrueType {};

template <typename T>
struct And<T> : T {};

template <typename T1, class... Tn>
struct And<T1, Tn...> : ConditionalT<T1::value, And<Tn...>, T1> {};

template <typename ...T>
inline constexpr bool AndV = And<T...>::value;

template <typename ...T>
struct Or : FalseType {};

template <typename T>
struct Or<T> : T {};

template <typename T1, typename... Tn>
struct Or<T1, Tn...> : ConditionalT<T1::value, T1, Or<Tn...>> {};

template <typename ...T>
inline constexpr bool OrV = Or<T...>::value;

template <typename T, typename ...Tn>
struct AreAllOf : And<IsSame<T, Tn>...> {};

template <typename T, typename ...Tn>
inline constexpr bool AreAllOfV = AreAllOf<T, Tn...>::value;

template <typename T, typename ...Tn>
struct IsAnyOf : Or<IsSame<T, Tn>...> {};

template <typename T, typename ...Tn>
inline constexpr bool IsAnyOfV = IsAnyOf<T, Tn...>::value;

template <typename T>
struct RemoveConst { using type = T; };

template <typename T>
struct RemoveConst<const T> { using type = T; };

template <typename T>
using RemoveConstT = typename RemoveConst<T>::type;

template <typename T>
struct AddConst { using type = const T; };

template <typename T>
using AddConstT = typename AddConst<T>::type;

template <typename T>
struct RemoveVolatile { using type = T; };

template <typename T>
struct RemoveVolatile<volatile T> { using type = T; };

template <typename T>
using RemoveVolatileT = typename RemoveVolatile<T>::type;

template <typename T>
struct AddVolatile { using type = volatile T; };

template <typename T>
using AddVolatileT = typename AddVolatile<T>::type;

template <typename T>
struct RemoveRef { using type = T; };

template <typename T>
struct RemoveRef<T &> { using type = T; };

template <typename T>
struct RemoveRef<T &&> { using type = T; };

template <typename T>
using RemoveRefT = typename RemoveRef<T>::type;

template <typename T>
using AddRef = std::add_lvalue_reference<T>;

template <typename T>
using AddRefT = typename AddRef<T>::type;

template <typename T>
using RemoveConstRef = RemoveRef<RemoveConstT<T>>;

template <typename T>
using RemoveConstRefT = typename RemoveConstRef<T>::type;

template <typename T>
using AddConstRef = AddRef<AddConstT<T>>;

template <typename T>
using AddConstRefT = typename AddConstRef<T>::type;

template <typename T>
using RemoveCV = RemoveConst<RemoveVolatileT<T>>;

template <typename T>
using RemoveCVT = typename RemoveCV<T>::type;

template <typename T>
struct AddCV { using type = const volatile T; };

template <typename T>
using AddCVT = typename AddCV<T>::type;

template <typename T>
struct RemoveCVRef : RemoveCV<RemoveRefT<T>> {};

template <typename T>
using RemoveCVRefT = typename RemoveCVRef<T>::type;

template <typename T>
struct AddCVRef : AddRef<AddCVT<T>> {};

template <typename T>
using AddCVRefT = typename AddCVRef<T>::type;

template <typename T>
struct IsPointer : FalseType {};

template <typename T>
struct IsPointer<T *> : TrueType {};

template <typename T>
struct IsPointer<T *const> : TrueType {};

template <typename T>
struct IsPointer<T *volatile> : TrueType {};

template <typename T>
struct IsPointer<T *const volatile> : TrueType {};

template <typename T>
inline constexpr bool IsPointerV = IsPointer<T>::value;

template <typename T>
struct RemovePointer { using type = T; };

template <typename T>
struct RemovePointer<T *> { using type = T; };

template <typename T>
struct RemovePointer<T *const> { using type = T; };

template <typename T>
struct RemovePointer<T *volatile> { using type = T; };

template <typename T>
struct RemovePointer<T *const volatile> { using type = T; };

template <typename T>
using RemovePointerT = typename RemovePointer<T>::type;

template <typename T>
using AddPointer = std::add_pointer<T>;

template <typename T>
using AddPointerT = typename AddPointer<T>::type;

template <typename T, bool = IsPointerV<T>>
struct __RemoveCVFromPointer { using type = T; };

template <typename T>
struct __RemoveCVFromPointer<T, true> { using type = AddPointerT<RemoveCVT<RemovePointerT<T>>>; };

template <typename T>
using RemoveCVFromPointer = __RemoveCVFromPointer<T>;

template <typename T>
using RemoveCVFromPointerT = typename RemoveCVFromPointer<T>::type;

template <typename T>
struct __IsIntegral : FalseType {};

template <>
struct __IsIntegral<bool> : TrueType {};

template <>
struct __IsIntegral<char> : TrueType {};

template <>
struct __IsIntegral<signed char> : TrueType {};

template <>
struct __IsIntegral<unsigned char> : TrueType {};

template <>
struct __IsIntegral<short> : TrueType {};

template <>
struct __IsIntegral<unsigned short> : TrueType {};

template <>
struct __IsIntegral<int> : TrueType {};

template <>
struct __IsIntegral<unsigned int> : TrueType {};

template <>
struct __IsIntegral<long> : TrueType {};

template <>
struct __IsIntegral<unsigned long> : TrueType {};

template <>
struct __IsIntegral<long long> : TrueType {};

template <>
struct __IsIntegral<unsigned long long> : TrueType {};

#ifdef FMT_INT128_SUPPORT
template <>
struct __IsIntegral<fmt_int128_t> : TrueType {};

template <>
struct __IsIntegral<fmt_uint128_t> : TrueType {};

#endif // FMT_INT128_SUPPORT

template <typename T>
using IsIntegral = __IsIntegral<RemoveCVT<T>>;

template <typename T>
inline constexpr bool IsIntegralV = IsIntegral<T>::value;

template <typename T>
struct __IsFloatingPoint : FalseType {};

template <>
struct __IsFloatingPoint<float> : TrueType {};

template <>
struct __IsFloatingPoint<double> : TrueType {};

#ifdef FMT_FLOAT16_SUPPORT
struct __IsFloatingPoint<fmt_float16_t> : TrueType {};

#endif // FMT_FLOAT16_SUPPORT

#ifdef FMT_BFLOAT16_SUPPORT
struct __IsFloatingPoint<fmt_bfloat16_t> : TrueType {};

#endif // FMT_BFLOAT16_SUPPORT

#ifdef FMT_FLOAT128_SUPPORT
struct __IsFloatingPoint<fmt_float128_t> : TrueType {};

#endif // FMT_FLOAT128_SUPPORT

template <typename T>
using IsFloatingPoint = __IsFloatingPoint<RemoveCVT<T>>;

template <typename T>
inline constexpr bool IsFloatingPointV = IsFloatingPoint<T>::value;

template <typename T>
using IsArithmetic = Or<IsIntegral<T>, IsFloatingPoint<T>>;

template <typename T>
inline constexpr bool IsArithmeticV = IsArithmetic<T>::value;

template <typename T, bool = IsArithmeticV<T> >
struct __IsSigned : BoolConstant<T(-1) < T(0)> {};

template <typename T>
struct __IsSigned<T, false> : FalseType {};

template <typename T>
using IsSigned = __IsSigned<RemoveCVT<T>>;

template <typename T>
inline constexpr bool IsSignedV = IsSigned<T>::value;

template <size_t>
struct SignedInt;

template <>
struct SignedInt<1U> { using type = int8_t; };

template <>
struct SignedInt<2U> { using type = int16_t; };

template <>
struct SignedInt<4U> { using type = int32_t; };

template <>
struct SignedInt<8U> { using type = int64_t; };

#ifdef FMT_INT128_SUPPORT
template <>
struct SignedInt<16U> { using type = fmt_int128_t; };
#endif

template <size_t W>
using SignedIntT = typename SignedInt<W>::type;

template <typename T>
struct __MakeSigned : SignedInt<sizeof(T)> {};

template <typename T>
struct __MakeSigned<const T> { using type = const SignedIntT<sizeof(T)>; };

template <typename T>
struct __MakeSigned<volatile T> { using type = volatile SignedIntT<sizeof(T)>; };

template <typename T>
struct __MakeSigned<const volatile T> { using type = const volatile SignedIntT<sizeof(T)>; };

template <typename T, EnableIfT<IsIntegralV<T>> = true>
using _MakeSigned = __MakeSigned<T>;

template <typename T>
using MakeSigned = _MakeSigned<T>;

template <typename T>
using MakeSignedT = typename MakeSigned<T>::type;

template <typename T, bool = IsArithmeticV<T>>
struct __IsUnsigned : Not<__IsSigned<T>> {};

template <typename T>
struct __IsUnsigned<T, false> : FalseType {};

template <typename T>
using IsUnsigned = __IsUnsigned<RemoveCVT<T>>;

template <typename T>
inline constexpr bool IsUnsignedV = IsUnsigned<T>::value;

template <size_t>
struct UnsignedInt;

template <>
struct UnsignedInt<1U> { using type = uint8_t; };

template <>
struct UnsignedInt<2U> { using type = uint16_t; };

template <>
struct UnsignedInt<4U> { using type = uint32_t; };

template <>
struct UnsignedInt<8U> { using type = uint64_t; };

#ifdef FMT_INT128_SUPPORT
template <>
struct UnsignedInt<16U> { using type = fmt_uint128_t; };
#endif

template <size_t W>
using UnsignedIntT = typename UnsignedInt<W>::type;

template <typename T>
struct __MakeUnsigned : UnsignedInt<sizeof(T)> {};

template <typename T>
struct __MakeUnsigned<const T> { using type = const UnsignedIntT<sizeof(T)>; };

template <typename T>
struct __MakeUnsigned<volatile T> { using type = volatile UnsignedIntT<sizeof(T)>; };

template <typename T>
struct __MakeUnsigned<const volatile T> { using type = const volatile UnsignedIntT<sizeof(T)>; };

template <typename T, EnableIfT<IsIntegralV<T>> = true>
using _MakeUnsigned = __MakeUnsigned<T>;

template <typename T>
using MakeUnsigned = _MakeUnsigned<T>;

template <typename T>
using MakeUnsignedT = typename MakeUnsigned<T>::type;

template <typename T>
struct __IsSmartPointer : FalseType {};

template <typename ...T>
struct __IsSmartPointer<std::unique_ptr<T...>> : TrueType {};

template <typename T>
struct __IsSmartPointer<std::shared_ptr<T>> : TrueType {};

template <typename T>
struct __IsSmartPointer<std::weak_ptr<T>> : TrueType {};

template <typename T>
using IsSmartPointer = __IsSmartPointer<RemoveConstRefT<T>>;

template <typename T>
inline constexpr bool IsSmartPointerV = IsSmartPointer<T>::value;

template <typename T>
struct __IsStringType : FalseType {};

template <>
struct __IsStringType<char *> : TrueType {};

template <>
struct __IsStringType<std::string> : TrueType {};

template <>
struct __IsStringType<std::string_view> : TrueType {};

template <typename T>
using IsStringType = __IsStringType<RemoveCVFromPointerT<RemoveConstRefT<T>>>;

template <typename T>
inline constexpr bool IsStringTypeV = IsStringType<T>::value;

FMT_END_NAMESPACE
