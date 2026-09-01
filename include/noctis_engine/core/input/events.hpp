#pragma once
#include "input_info.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"


namespace NoctisEngine
{
    
/// @brief A keyboard event
struct KeyboardInputEvent
{
    /// @brief The event's key
    PhysicalKey key;

    /// @brief The event's input info
    /// @warning The InputInfo::state cannot be InputInfo::State::HELD 
    /// or InputInfo::State::UP 
    InputInfo info;
};

/// @brief A mouse button event
struct MouseButtonEvent
{
    /// @brief The event's mouse button
    MouseButton btn;

    /// @brief The event's input info
    /// @warning The InputInfo::state cannot be InputInfo::State::HELD 
    /// or InputInfo::State::UP 
    InputInfo info;
};

} // namespace NoctisEngine
