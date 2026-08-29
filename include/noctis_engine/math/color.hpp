#pragma once
#include <stdint.h>

#include "../noctis_engine.hpp"
#include "math.hpp"


namespace NoctisEngine
{
    
/// @brief A class representing a RGBA color
class Color {
public:
    /// @brief Default constructor, all components are set to 0
    constexpr Color()
        : m_r(0)
        , m_g(0) 
        , m_b(0) 
        , m_a(0)
    {}
    
    /// @brief Creates a color
    /// @param red The color's red component
    /// @param green The color's green component
    /// @param blue The color's blue component
    /// @param alpha The color's alpha component. Default 255
    constexpr Color(
        std::uint8_t red, 
        std::uint8_t green, 
        std::uint8_t blue, 
        std::uint8_t alpha = 255) 
        : m_r(red)
        , m_g(green) 
        , m_b(blue) 
        , m_a(alpha) 
    {}

    /// @return This color's red component 
    constexpr auto red() const -> std::uint8_t { return m_r; }

    /// @return This color's green component 
    constexpr auto green() const -> std::uint8_t { return m_g; }

    /// @return This color's blue component 
    constexpr auto blue() const -> std::uint8_t { return m_b; }

    /// @return This color's alpha component 
    constexpr auto alpha() const -> std::uint8_t { return m_a; }

    /// @return This color's red component as a float in the [0, 1] range
    constexpr auto red_f() const -> float { return (float)m_r / 255; }

    /// @return This color's green component as a float in the [0, 1] range
    constexpr auto green_f() const -> float { return (float)m_g / 255; }

    /// @return This color's blue component as a float in the [0, 1] range
    constexpr auto blue_f() const -> float { return (float)m_b / 255; }

    /// @return This color's alpha component as a float in the [0, 1] range
    constexpr auto alpha_f() const -> float { return (float)m_a / 255; }

    /// @brief Converts this color to a uint32
    /// @return The uint32
    constexpr auto to_RGBA_int() const -> std::uint32_t
    {
        return (static_cast<std::uint32_t>(m_r) << 24)
             | (static_cast<std::uint32_t>(m_g) << 16)
             | (static_cast<std::uint32_t>(m_b) << 8)
             | (static_cast<std::uint32_t>(m_a));
    }

    /// @brief Converts this color to a glm::vec4 containing all the 
    /// components' floating point values
    /// @return The glm::vec4 
    constexpr auto to_floats() const -> glm::vec4 
    { 
        return glm::vec4{red_f(), green_f(), blue_f(), alpha_f()};
    }

private:
    std::uint8_t m_r{}, m_g{}, m_b{}, m_a{};
};

/// @brief Converts a RGBA color to 4 floats (red, green, blue and alpha) in the range [0, 1]
/// @param color The RGBA color
/// @return The 4 floats
constexpr auto RGBA_to_floats(std::uint32_t color) -> glm::vec4
{
    float r = static_cast<float>((color >> 24)  & 0xFF) / 255.0f;
    float g = static_cast<float>((color >> 16)  & 0xFF) / 255.0f;
    float b = static_cast<float>((color >> 8)   & 0xFF) / 255.0f;
    float a = static_cast<float>((color)        & 0xFF) / 255.0f;

    return glm::vec4{ r, g, b, a };

}

} // namespace NoctisEngine