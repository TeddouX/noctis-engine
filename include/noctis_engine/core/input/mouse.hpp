#pragma once
#include "../../math/math.hpp"


namespace NoctisEngine
{

/// @brief Represents a mouse movement event
struct MouseMouvement 
{
    /// @brief The mouse pointer's current position
    glm::vec2 pos{0};
    
    /// @brief How much the mouse pointer's position changed 
    /// over the last frame
    glm::vec2 delta{0};
};

/// @brief Represents any recognized mouse button
enum class MouseButton : int
{
    LEFT        = 0,
    RIGHT       = 1,
    MIDDLE      = 2,
    BUTTON_4    = 3,
    BUTTON_5    = 4,
    BUTTON_6    = 5,
    BUTTON_7    = 6,
    BUTTON_8    = 7,

    MAX_MOUSE_BUTTON
};

/// @brief The number of recognized mouse buttons
constexpr int NUM_MOUSE_BUTTONS = static_cast<int>(MouseButton::MAX_MOUSE_BUTTON) - 1;

/// @brief Returns the ordinal (number associated to a key) of a mouse button
constexpr auto ordinal(MouseButton mouse_btn) -> int 
{
    return static_cast<int>(mouse_btn);
}

} // namespace NoctisEngine
