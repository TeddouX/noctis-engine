#pragma once
#include <cstdint>
#include <vector>
#include <memory>

#include "event_receiver.hpp"


namespace NoctisEngine::Core
{
    
/// @brief This singleton handles event layering
class EventStack
{
public:
    EventStack() = delete;

    /// @brief Adds an event receiver on the stop of the stack
    /// @param receiver The event receiver
    static auto push_receiver(std::shared_ptr<IEventReceiver> receiver) -> void;

    /// @brief Pops the topmost event receiver from the stack
    static auto pop_receiver() -> void;

private:
    friend class InputHandler;

    inline static std::vector<std::shared_ptr<IEventReceiver>> event_receiver_stack_;

    static auto keyboard_event(const KeyboardInputEvent &event) -> void;
    static auto mouse_button_event(const MouseButtonEvent &event) -> void;
    static auto mouse_move(const MouseMouvement &mouse_movement) -> void;
};

} // namespace NoctisEngine::Core
