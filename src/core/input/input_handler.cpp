#include <noctis_engine/core/input/input_handler.hpp>

#include <GLFW/glfw3.h>

#include <noctis_engine/core/logging.hpp>


namespace NoctisEngine::Core
{
    
auto InputHandler::key_pressed(PhysicalKey key) -> bool
{
    return key_states_[ordinal(key)].state == InputInfo::State::PRESSED
        | key_states_[ordinal(key)].state == InputInfo::State::HELD;
}

auto InputHandler::key_released(PhysicalKey key) -> bool
{
    return key_states_[ordinal(key)].state == InputInfo::State::RELEASED
        | key_states_[ordinal(key)].state == InputInfo::State::UP;
}

auto InputHandler::key_held(PhysicalKey key) -> bool
{
    return key_states_[ordinal(key)].state == InputInfo::State::HELD;
}

auto InputHandler::key_just_released(PhysicalKey key) -> bool
{
    return key_states_[ordinal(key)].state == InputInfo::State::RELEASED;
}

auto InputHandler::key_just_pressed(PhysicalKey key) -> bool
{
    return key_states_[ordinal(key)].state == InputInfo::State::PRESSED;
}

auto InputHandler::key_mods(PhysicalKey key) -> Modifier
{
    if (key_pressed(key))
        return key_states_[ordinal(key)].mods;
    return Modifier::NONE;
}

auto InputHandler::mouse_mouvement() -> MouseMouvement
{
    return last_mouse_mvt_;
}

auto InputHandler::mouse_button_just_pressed(MouseButton mb) -> bool
{
    return mouse_buttons_[ordinal(mb)].state == InputInfo::State::PRESSED;
}

auto InputHandler::mouse_button_pressed(MouseButton mb) -> bool
{
    return mouse_buttons_[ordinal(mb)].state == InputInfo::State::PRESSED
        | mouse_buttons_[ordinal(mb)].state == InputInfo::State::HELD;
}

auto InputHandler::mouse_button_just_released(MouseButton mb) -> bool
{
    return mouse_buttons_[ordinal(mb)].state == InputInfo::State::RELEASED;
}

auto InputHandler::mouse_button_released(MouseButton mb) -> bool
{
    return mouse_buttons_[ordinal(mb)].state == InputInfo::State::RELEASED
        | mouse_buttons_[ordinal(mb)].state == InputInfo::State::UP;
}

auto InputHandler::mouse_button_held(MouseButton mb) -> bool
{
    return key_states_[ordinal(mb)].state == InputInfo::State::HELD;
}

auto InputHandler::update() -> void
{
    for (std::size_t key_ord : dirty_keys_)
        update_state(key_states_[key_ord]);

    for (InputInfo &mouse_btn_state : mouse_buttons_)
        update_state(mouse_btn_state);

    last_mouse_mvt_.delta.x = 0;
    last_mouse_mvt_.delta.y = 0;
}

auto InputHandler::update_state(InputInfo &state) -> bool
{
    if (state.state == InputInfo::State::UP || state.state == InputInfo::State::HELD)
        return false;
    
    if (state.state == InputInfo::State::RELEASED) 
    {
        state.state = InputInfo::State::UP;
        state.mods = Modifier::NONE;
    
        return true;
    }
    else if (state.state == InputInfo::State::PRESSED)
    {
        state.state = InputInfo::State::HELD;
     
        return true;
    }

    return false;
}

auto InputHandler::glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) -> void
{
    if (action == GLFW_REPEAT)
        return;

    PhysicalKey phys_key = static_cast<PhysicalKey>(key);
    Modifier modifiers = static_cast<Modifier>(mods);
    InputInfo::State key_state = InputInfo::State::UP;
    int key_ord = ordinal(phys_key);
    
    if (action == GLFW_PRESS && key_released(phys_key)) 
    {
        key_state = InputInfo::State::PRESSED;
        dirty_keys_.push_back(key_ord);
    }
    else if (action == GLFW_RELEASE) 
    {
        key_state = InputInfo::State::RELEASED;
        dirty_keys_.push_back(key_ord);
    }

    key_states_[key_ord] = InputInfo{
        .state = key_state,
        .mods = modifiers,
    };

    EventStack::keyboard_event(KeyboardInputEvent{
        .key = phys_key,
        .info = key_states_[key_ord],
    });
}

auto InputHandler::glfw_cursor_pos_callback(GLFWwindow *window, double xPos, double yPos) -> void
{
    glm::vec2 pos{xPos, yPos};

    MouseMouvement new_mouse_mvt{};
    new_mouse_mvt.delta = pos - last_mouse_mvt_.pos;
    new_mouse_mvt.pos = pos;

    last_mouse_mvt_ = new_mouse_mvt;

    EventStack::mouse_move(new_mouse_mvt);
}

auto InputHandler::glfw_mouse_btn_callback(GLFWwindow *window, int button, int action, int mods) -> void
{
    MouseButton mouse_btn = static_cast<MouseButton>(button);
    Modifier modifiers = static_cast<Modifier>(mods);
    InputInfo::State btn_state = InputInfo::State::UP;
    int btn_ord = ordinal(mouse_btn);
    
    if (action == GLFW_PRESS && mouse_button_released(mouse_btn))
    {
        btn_state = InputInfo::State::PRESSED;
        CORE_LOGGER.debug("{} pressed", button);
    }
    else if (action == GLFW_RELEASE)
    {
        btn_state = InputInfo::State::RELEASED;
        CORE_LOGGER.debug("{} released", button);
    }

    mouse_buttons_[btn_ord] = InputInfo{
        .state = btn_state,
        .mods = modifiers,
    };

    EventStack::mouse_button_event(MouseButtonEvent{
        .btn = mouse_btn,
        .info = mouse_buttons_[btn_ord],
    });
}
    
} // namespace NoctisEngine::Core
