#pragma once
#include <stdint.h>

#include "../noctis_engine.hpp"
#include "math.hpp"


namespace NoctisEngine
{
    
class Color {
public:
    constexpr Color()
        : m_r(0)
        , m_g(0) 
        , m_b(0) 
        , m_a(0)
    {}
    
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

    static const Color BLACK;

    constexpr auto red() const -> std::uint8_t { return m_r; }
    constexpr auto green() const -> std::uint8_t { return m_g; }
    constexpr auto blue() const -> std::uint8_t { return m_b; }
    constexpr auto alpha() const -> std::uint8_t { return m_a; }

    constexpr auto red_f() const -> float { return (float)m_r / 255; }
    constexpr auto green_f() const -> float { return (float)m_g / 255; }
    constexpr auto blue_f() const -> float { return (float)m_b / 255; }
    constexpr auto alpha_f() const -> float { return (float)m_a / 255; }

    constexpr auto to_RGBA_int() const -> std::uint32_t
    {
        return (static_cast<std::uint32_t>(m_r) << 24)
             | (static_cast<std::uint32_t>(m_g) << 16)
             | (static_cast<std::uint32_t>(m_b) << 8)
             | (static_cast<std::uint32_t>(m_a));
    }

    constexpr auto to_floats() const -> glm::vec4 
    { 
        return glm::vec4{red_f(), green_f(), blue_f(), alpha_f()};
    }

private:
    std::uint8_t m_r{}, m_g{}, m_b{}, m_a{};
};

inline const Color Color::BLACK{0, 0, 0, 255};

} // namespace NoctisEngine