#pragma once
#include <type_traits>

namespace NoctisEngine 
{

template <typename Ty_>
concept RealNumber = !std::is_same_v<Ty_, bool> 
    && std::is_arithmetic_v<Ty_>; 

template <typename Ty_>
concept SignedRealNumber = !std::is_same_v<Ty_, bool> 
    && std::is_arithmetic_v<Ty_> 
    && (!std::is_integral_v<Ty_> || std::is_signed_v<Ty_>);


/// @brief Returns -1 if the scalar is strictly negative, 1 otherwise
template <RealNumber RealNumber_>
inline constexpr auto sign(RealNumber_ scalar) -> float
{
    return scalar < 0 ? -1 : 1;
}

}