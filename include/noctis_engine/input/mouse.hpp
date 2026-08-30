#pragma once
#include "../math/math.hpp"


namespace NoctisEngine
{

/// @brief Represents a mouse movement event
struct MouseMouvement 
{
    /// @brief The mouse pointer's current position
    glm::vec2 pos;
    /// @brief How much the mouse pointer's position changed 
    /// over the last frame
    glm::vec2 delta;
};

/// @brief Represents any recognized mouse button
enum class MouseButton : int
{
    LEFT,
    RIGHT,
    MIDDLE,

    BUTTON_3,
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_7,

    MAX_MOUSE_BUTTON
};

/// @brief The number of recognized mouse buttons
constexpr int NUM_MOUSE_BUTTONS = static_cast<int>(MouseButton::MAX_MOUSE_BUTTON) - 1;

} // namespace NoctisEngine
