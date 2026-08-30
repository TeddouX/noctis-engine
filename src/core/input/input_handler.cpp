#include <noctis_engine/core/input/input_handler.hpp>

#include <GLFW/glfw3.h>

#include <noctis_engine/core/logging.hpp>


namespace NoctisEngine::Core
{
    
auto InputHandler::key_pressed(PhysicalKey key) -> bool
{
    return key_states_[ordinal(key)].state == InputState::State::PRESSED
        | key_states_[ordinal(key)].state == InputState::State::HELD;
}

auto InputHandler::key_released(PhysicalKey key) -> bool
{
    return key_states_[ordinal(key)].state == InputState::State::RELEASED
        | key_states_[ordinal(key)].state == InputState::State::UP;
}

auto InputHandler::key_held(PhysicalKey key) -> bool
{
    return key_states_[ordinal(key)].state == InputState::State::HELD;
}

auto InputHandler::key_just_released(PhysicalKey key) -> bool
{
    return key_states_[ordinal(key)].state == InputState::State::RELEASED;
}

auto InputHandler::key_just_pressed(PhysicalKey key) -> bool
{
    return key_states_[ordinal(key)].state == InputState::State::PRESSED;
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
    return mouse_buttons_[ordinal(mb)].state == InputState::State::PRESSED;
}

auto InputHandler::mouse_button_pressed(MouseButton mb) -> bool
{
    return mouse_buttons_[ordinal(mb)].state == InputState::State::PRESSED
        | mouse_buttons_[ordinal(mb)].state == InputState::State::HELD;
}

auto InputHandler::mouse_button_just_released(MouseButton mb) -> bool
{
    return mouse_buttons_[ordinal(mb)].state == InputState::State::RELEASED;
}

auto InputHandler::mouse_button_released(MouseButton mb) -> bool
{
    return mouse_buttons_[ordinal(mb)].state == InputState::State::RELEASED
        | mouse_buttons_[ordinal(mb)].state == InputState::State::UP;
}

auto InputHandler::mouse_button_held(MouseButton mb) -> bool
{
    return key_states_[ordinal(mb)].state == InputState::State::HELD;
}

auto InputHandler::update() -> void
{
    for (std::size_t key_ord : dirty_keys_)
        update_state(key_states_[key_ord]);

    for (InputState &mouse_btn_state : mouse_buttons_)
        update_state(mouse_btn_state);

    last_mouse_mvt_.delta.x = 0;
    last_mouse_mvt_.delta.y = 0;
}

auto InputHandler::update_state(InputState &state) -> void
{
    if (state.state == InputState::State::UP || state.state == InputState::State::HELD)
        return;
    
    if (state.state == InputState::State::RELEASED) 
    {
        state.state = InputState::State::UP;
        state.mods = Modifier::NONE;
    }
    else if (state.state == InputState::State::PRESSED)
    {
        state.state = InputState::State::HELD;
    }
}

auto InputHandler::glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) -> void
{
    if (action == GLFW_REPEAT)
        return;

    PhysicalKey phys_key = static_cast<PhysicalKey>(key);
    Modifier modifiers = static_cast<Modifier>(mods);
    InputState::State key_state = InputState::State::UP;
    int key_ord = ordinal(phys_key);
    
    if (action == GLFW_PRESS && key_released(phys_key)) 
    {
        key_state = InputState::State::PRESSED;
        dirty_keys_.push_back(key_ord);
    }
    else if (action == GLFW_RELEASE) 
    {
        key_state = InputState::State::RELEASED;
        dirty_keys_.push_back(key_ord);
    }

    key_states_[key_ord] = InputState{
        .state = key_state,
        .mods = modifiers,
    };
}

auto InputHandler::glfw_cursor_pos_callback(GLFWwindow *window, double xPos, double yPos) -> void
{
    glm::vec2 pos{xPos, yPos};

    MouseMouvement new_mouse_mvt{};
    new_mouse_mvt.delta = pos - last_mouse_mvt_.pos;
    new_mouse_mvt.pos = pos;

    last_mouse_mvt_ = new_mouse_mvt;
}

auto InputHandler::glfw_mouse_btn_callback(GLFWwindow *window, int button, int action, int mods) -> void
{
    MouseButton mouse_btn = static_cast<MouseButton>(button);
    Modifier modifiers = static_cast<Modifier>(mods);
    InputState::State btn_state = InputState::State::UP;
    int btn_ord = ordinal(mouse_btn);
    
    if (action == GLFW_PRESS && mouse_button_released(mouse_btn))
    {
        btn_state = InputState::State::PRESSED;
        CORE_LOGGER.debug("{} pressed", button);
    }
    else if (action == GLFW_RELEASE)
    {
        btn_state = InputState::State::RELEASED;
        CORE_LOGGER.debug("{} released", button);
    }

    mouse_buttons_[btn_ord] = InputState{
        .state = btn_state,
        .mods = modifiers,
    };
}
    
} // namespace NoctisEngine::Core
