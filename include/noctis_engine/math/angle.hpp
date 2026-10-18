#pragma once
#include <numbers>


namespace NoctisEngine 
{

/// @brief Multiply by this number to convert from radians to degrees
inline constexpr float RAD_TO_DEG = 180 / std::numbers::pi_v<float>;
/// @brief Multiply by this number to convert from degrees to radians
inline constexpr float DEG_TO_RAD = std::numbers::pi_v<float> / 180;

/// @brief Converts an angle in degrees to radians
inline constexpr auto radians(float degrees) -> float
{
    return degrees * DEG_TO_RAD;
}

/// @brief Converts an angle in radians to degrees
inline constexpr auto degrees(float radians) -> float
{
    return radians * RAD_TO_DEG;
}

}