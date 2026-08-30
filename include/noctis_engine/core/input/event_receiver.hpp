#pragma once
#include "events.hpp"


namespace NoctisEngine::Core
{
    
/// @brief The interface for classes that receive events from the event stack
class IEventReceiver
{
public:
    virtual ~IEventReceiver() = default;

    /// @brief Called when a keyboard event is received
    /// @param event The event
    /// @return True if this receiver consumes it, false otherwise
    /// @warning The InputInfo::state cannot be InputInfo::State::HELD 
    /// or InputInfo::State::UP 
    virtual auto on_keyboard_event(const KeyboardInputEvent &event) -> bool 
    {
        return false;
    }

    /// @brief Called when a mouse button was pressed
    /// @param event The event
    /// @return True if this receiver consumes it, false otherwise
    /// @warning The InputInfo::state cannot be InputInfo::State::HELD 
    /// or InputInfo::State::UP 
    virtual auto on_mouse_button_event(const MouseButtonEvent &event) -> bool 
    {
        return false;
    }

    /// @brief Called when the mouse moved
    /// @param event The mouse movement data
    /// @return True if this receiver consumes it, false otherwise
    virtual auto on_mouse_move(const MouseMouvement &mouse_movement) -> bool 
    {
        return false;
    }
};

} // namespace NoctisEngine::Core
