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
struct EnableIf<true> { using Type = bool; };

template <bool B>
using EnableIfT = typename EnableIf<B>::Type;

template <typename T, T V>
struct IntegralConstant
{
  using ValueT = T;
  using Type = IntegralConstant;

  static constexpr const ValueT Value = V;

  constexpr operator ValueT(void) const noexcept   { return Value; }
  constexpr ValueT operator()(void) const noexcept { return Value; }
};

template <bool B>
struct BoolConstant : IntegralConstant<bool, B> {};

using FalseType = BoolConstant<false>;

using TrueType = BoolConstant<true>;

template <typename T1, typename T2>
struct IsSame : FalseType {};

template <typename T>
struct IsSame<T, T> : TrueType {};

template <typename T1, typename T2>
inline constexpr bool IsSameV = IsSame<T1, T2>::Value;

template <bool B, typename TrueT, typename FalseT>
struct Conditional { using Type = FalseT; };

template <typename TrueT, typename FalseT>
struct Conditional<true, TrueT, FalseT> { using Type = TrueT; };

template <bool B, typename TrueT, typename FalseT>
using ConditionalT = typename Conditional<B, TrueT, FalseT>::Type;

template <typename T>
using Not = BoolConstant<!T::Value>;

template <typename T>
inline constexpr bool NotV = Not<T>::Value;

template <typename ...T>
struct And : TrueType {};

template <typename T>
struct And<T> : T {};

template <typename T1, class... Tn>
struct And<T1, Tn...> : ConditionalT<T1::Value, And<Tn...>, T1> {};

template <typename ...T>
inline constexpr bool AndV = And<T...>::Value;

template <typename ...T>
struct Or : FalseType {};

template <typename T>
struct Or<T> : T {};

template <typename T1, typename... Tn>
struct Or<T1, Tn...> : ConditionalT<T1::Value, T1, Or<Tn...>> {};

template <typename ...T>
inline constexpr bool OrV = Or<T...>::Value;

template <typename T, typename ...Tn>
struct AreAllOf : And<IsSame<T, Tn>...> {};

template <typename T, typename ...Tn>
inline constexpr bool AreAllOfV = AreAllOf<T, Tn...>::Value;

template <typename T, typename ...Tn>
struct IsAnyOf : Or<IsSame<T, Tn>...> {};

template <typename T, typename ...Tn>
inline constexpr bool IsAnyOfV = IsAnyOf<T, Tn...>::Value;

template <typename T>
struct RemoveConst { using Type = T; };

template <typename T>
struct RemoveConst<const T> { using Type = T; };

template <typename T>
using RemoveConstT = typename RemoveConst<T>::Type;

template <typename T>
struct AddConst { using Type = const T; };

template <typename T>
using AddConstT = typename AddConst<T>::Type;

template <typename T>
struct RemoveVolatile { using Type = T; };

template <typename T>
struct RemoveVolatile<volatile T> { using Type = T; };

template <typename T>
using RemoveVolatileT = typename RemoveVolatile<T>::Type;

template <typename T>
struct AddVolatile { using Type = volatile T; };

template <typename T>
using AddVolatileT = typename AddVolatile<T>::Type;

template <typename T>
struct RemoveRef { using Type = T; };

template <typename T>
struct RemoveRef<T &> { using Type = T; };

template <typename T>
struct RemoveRef<T &&> { using Type = T; };

template <typename T>
using RemoveRefT = typename RemoveRef<T>::Type;

template <typename T>
struct AddRef { using Type = std::add_lvalue_reference_t<T>; };

template <typename T>
using AddRefT = typename AddRef<T>::Type;

template <typename T>
using RemoveConstRef = RemoveRef<RemoveConstT<T>>;

template <typename T>
using RemoveConstRefT = typename RemoveConstRef<T>::Type;

template <typename T>
using AddConstRef = AddRef<AddConstT<T>>;

template <typename T>
using AddConstRefT = typename AddConstRef<T>::Type;

template <typename T>
using RemoveCV = RemoveConst<RemoveVolatileT<T>>;

template <typename T>
using RemoveCVT = typename RemoveCV<T>::Type;

template <typename T>
struct AddCV { using Type = const volatile T; };

template <typename T>
using AddCVT = typename AddCV<T>::Type;

template <typename T>
struct RemoveCVRef : RemoveCV<RemoveRefT<T>> {};

template <typename T>
using RemoveCVRefT = typename RemoveCVRef<T>::Type;

template <typename T>
struct AddCVRef : AddRef<AddCVT<T>> {};

template <typename T>
using AddCVRefT = typename AddCVRef<T>::Type;

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
inline constexpr bool IsPointerV = IsPointer<T>::Value;

template <typename T>
struct RemovePointer { using Type = T; };

template <typename T>
struct RemovePointer<T *> { using Type = T; };

template <typename T>
struct RemovePointer<T *const> { using Type = T; };

template <typename T>
struct RemovePointer<T *volatile> { using Type = T; };

template <typename T>
struct RemovePointer<T *const volatile> { using Type = T; };

template <typename T>
using RemovePointerT = typename RemovePointer<T>::Type;

template <typename T>
struct AddPointer { using Type = std::add_pointer_t<T>; };

template <typename T>
using AddPointerT = typename AddPointer<T>::Type;

template <typename T, bool = IsPointerV<T>>
struct __RemoveCVFromPointer { using Type = T; };

template <typename T>
struct __RemoveCVFromPointer<T, true> { using Type = AddPointerT<RemoveCVT<RemovePointerT<T>>>; };

template <typename T>
using RemoveCVFromPointer = __RemoveCVFromPointer<T>;

template <typename T>
using RemoveCVFromPointerT = typename RemoveCVFromPointer<T>::Type;

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
struct __IsIntegral<FmtInt128> : TrueType {};

template <>
struct __IsIntegral<FmtUInt128> : TrueType {};

#endif // FMT_INT128_SUPPORT

template <typename T>
using IsIntegral = __IsIntegral<RemoveCVT<T>>;

template <typename T>
inline constexpr bool IsIntegralV = IsIntegral<T>::Value;

template <typename T>
struct __IsFloatingPoint : FalseType {};

#ifdef FMT_FLOAT16_SUPPORT
template <>
struct __IsFloatingPoint<FmtFloat16> : TrueType {};
#endif // FMT_FLOAT16_SUPPORT

template <>
struct __IsFloatingPoint<float> : TrueType {};

template <>
struct __IsFloatingPoint<double> : TrueType {};

template <>
struct __IsFloatingPoint<long double> : TrueType {};

#ifdef FMT_FLOAT80_SUPPORT
template <>
struct __IsFloatingPoint<FmtFloat80> : TrueType {};
#endif // FMT_FLOAT80_SUPPORT

#ifdef FMT_FLOAT128_SUPPORT
template <>
struct __IsFloatingPoint<FmtFloat128> : TrueType {};
#endif // FMT_FLOAT128_SUPPORT

template <typename T>
using IsFloatingPoint = __IsFloatingPoint<RemoveCVT<T>>;

template <typename T>
inline constexpr bool IsFloatingPointV = IsFloatingPoint<T>::Value;

template <typename T>
using IsArithmetic = Or<IsIntegral<T>, IsFloatingPoint<T>>;

template <typename T>
inline constexpr bool IsArithmeticV = IsArithmetic<T>::Value;

template <typename T, bool = IsArithmeticV<T> >
struct __IsSigned : BoolConstant<T(-1) < T(0)> {};

template <typename T>
struct __IsSigned<T, false> : FalseType {};

template <typename T>
using IsSigned = __IsSigned<RemoveCVT<T>>;

template <typename T>
inline constexpr bool IsSignedV = IsSigned<T>::Value;

template <size_t>
struct SignedInt;

template <>
struct SignedInt<1U> { using Type = int8_t; };

template <>
struct SignedInt<2U> { using Type = int16_t; };

template <>
struct SignedInt<4U> { using Type = int32_t; };

template <>
struct SignedInt<8U> { using Type = int64_t; };

#ifdef FMT_INT128_SUPPORT
template <>
struct SignedInt<16U> { using Type = FmtInt128; };
#endif

template <size_t W>
using SignedIntT = typename SignedInt<W>::Type;

template <typename T>
struct __MakeSigned : SignedInt<sizeof(T)> {};

template <typename T>
struct __MakeSigned<const T> { using Type = const SignedIntT<sizeof(T)>; };

template <typename T>
struct __MakeSigned<volatile T> { using Type = volatile SignedIntT<sizeof(T)>; };

template <typename T>
struct __MakeSigned<const volatile T> { using Type = const volatile SignedIntT<sizeof(T)>; };

template <typename T, EnableIfT<IsIntegralV<T>> = true>
using _MakeSigned = __MakeSigned<T>;

template <typename T>
using MakeSigned = _MakeSigned<T>;

template <typename T>
using MakeSignedT = typename MakeSigned<T>::Type;

template <typename T, bool = IsArithmeticV<T>>
struct __IsUnsigned : Not<__IsSigned<T>> {};

template <typename T>
struct __IsUnsigned<T, false> : FalseType {};

template <typename T>
using IsUnsigned = __IsUnsigned<RemoveCVT<T>>;

template <typename T>
inline constexpr bool IsUnsignedV = IsUnsigned<T>::Value;

template <size_t>
struct UnsignedInt;

template <>
struct UnsignedInt<1U> { using Type = uint8_t; };

template <>
struct UnsignedInt<2U> { using Type = uint16_t; };

template <>
struct UnsignedInt<4U> { using Type = uint32_t; };

template <>
struct UnsignedInt<8U> { using Type = uint64_t; };

#ifdef FMT_INT128_SUPPORT
template <>
struct UnsignedInt<16U> { using Type = FmtUInt128; };
#endif

template <size_t W>
using UnsignedIntT = typename UnsignedInt<W>::Type;

template <typename T>
struct __MakeUnsigned : UnsignedInt<sizeof(T)> {};

template <typename T>
struct __MakeUnsigned<const T> { using Type = const UnsignedIntT<sizeof(T)>; };

template <typename T>
struct __MakeUnsigned<volatile T> { using Type = volatile UnsignedIntT<sizeof(T)>; };

template <typename T>
struct __MakeUnsigned<const volatile T> { using Type = const volatile UnsignedIntT<sizeof(T)>; };

template <typename T, EnableIfT<IsIntegralV<T>> = true>
using _MakeUnsigned = __MakeUnsigned<T>;

template <typename T>
using MakeUnsigned = _MakeUnsigned<T>;

template <typename T>
using MakeUnsignedT = typename MakeUnsigned<T>::Type;

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
inline constexpr bool IsSmartPointerV = IsSmartPointer<T>::Value;

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
inline constexpr bool IsStringTypeV = IsStringType<T>::Value;

FMT_END_NAMESPACE
