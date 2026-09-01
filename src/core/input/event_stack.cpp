#include <noctis_engine/core/input/event_stack.hpp>

#include <ranges>

#include <noctis_engine/core/logging.hpp>


namespace NoctisEngine
{
    
auto EventStack::push_receiver(std::shared_ptr<IEventReceiver> receiver) -> void
{
    event_receiver_stack_.push_back(receiver);
}

auto EventStack::pop_receiver() -> void
{
    if (event_receiver_stack_.empty())
    {
        CORE_LOGGER.error("Can't pop receiver from an empty stack");
        return;
    }

    event_receiver_stack_.pop_back();
}

auto EventStack::keyboard_event(const KeyboardInputEvent &event) -> void
{
    for (const auto &receiver : event_receiver_stack_ | std::views::reverse)
    {
        if (receiver->on_keyboard_event(event))
            break;
    }
}

auto EventStack::mouse_button_event(const MouseButtonEvent &event) -> void
{
    for (const auto &receiver : event_receiver_stack_ | std::views::reverse)
    {
        if (receiver->on_mouse_button_event(event))
            break;
    }
}

auto EventStack::mouse_move(const MouseMouvement &mouse_movement) -> void
{
    for (const auto &receiver : event_receiver_stack_ | std::views::reverse)
    {
        if (receiver->on_mouse_move(mouse_movement))
            break;
    }
}

} // namespace NoctisEngine
