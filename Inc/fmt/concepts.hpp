#pragma once

#include <type_traits>

namespace fmt
{

template<typename _Ty>
concept integral = (std::is_integral_v<_Ty>) && (sizeof(_Ty) <= 8U);

template<typename _Ty>
concept floating_point = (std::is_floating_point_v<_Ty>) && (sizeof(_Ty) <= 8U);

} // namespace fmt
