#pragma once
#include <variant>

#include "event_stack.hpp"
#include "keyboard.hpp"
#include "input_info.hpp"
#include "mouse.hpp"


struct GLFWwindow;


/// @brief This namespace containes everything implemented by the engine for input handling
namespace NoctisEngine::Core
{

/// @brief This is the singleton that handles all input processing
class InputHandler
{
public:
    InputHandler() = delete;

    /// @brief Checks if a physical key is pressed
    /// @param key The physical key that should be checked
    /// @return true if is the physical key is pressed
    static auto key_pressed(PhysicalKey key) -> bool;

    /// @brief Checks if a physical key is released
    /// @param key The physical key that should be checked
    /// @return true if is the physical key is v
    static auto key_released(PhysicalKey key) -> bool;

    /// @brief Checks if a physical key is held, but wasn't pressed in this frame
    /// @param key The physical key that should be checked
    /// @return true if is the physical key is held, but wasn't pressed in this frame
    static auto key_held(PhysicalKey key) -> bool;

    /// @brief Checks if a physical key was released in this frame
    /// @param key The physical key that should be checked
    /// @return true if is the physical key was released in this frame
    static auto key_just_released(PhysicalKey key) -> bool;

    /// @brief Checks if a physical key was pressed in this frame
    /// @param key The physical key that should be checked
    /// @return true if is the physical key was pressed in this frame
    static auto key_just_pressed(PhysicalKey key) -> bool;

    /// @brief Gets a pressed key's modifiers
    /// @param key The physical key that should be checked
    /// @return None if the key isn't pressed or wasn't pressed with any modifiers, 
    /// the modifiers otherwise
    static auto key_mods(PhysicalKey key) -> Modifier;

    /// @brief Gets this frame's mouse movement
    /// @return This frame's mouse movement
    static auto mouse_mouvement() -> MouseMouvement;

    /// @brief Checks if a mouse button was pressed in this frame
    /// @param key The mouse button that should be checked
    /// @return true if is the mouse button was pressed in this frame
    static auto mouse_button_just_pressed(MouseButton mb) -> bool;

    /// @brief Checks if a mouse button is pressed
    /// @param key The mouse button that should be checked
    /// @return true if is the mouse button is pressed
    static auto mouse_button_pressed(MouseButton mb) -> bool;

    /// @brief Checks if a mouse button was released in this frame
    /// @param key The mouse button that should be checked
    /// @return true if is the mouse button is released in this frame
    static auto mouse_button_just_released(MouseButton mb) -> bool;

    /// @brief Checks if a mouse button is released
    /// @param key The mouse button that should be checked
    /// @return true if is the mouse button is released
    static auto mouse_button_released(MouseButton mb) -> bool;

    /// @brief Checks if a mouse button is held, but not pressed in this frame
    /// @param key The mouse button that should be checked
    /// @return true if is the mouse button is held, but not pressed in this frame
    static auto mouse_button_held(MouseButton mb) -> bool;

    /// @brief All the possible input action types
    using InputActions = std::variant<
        PhysicalKey, 
        MouseButton
    >;

    /// @brief Registers an action
    /// @param name The action's name
    /// @param name The input actions this action will be triggered by
    static auto register_action(std::string_view name, const std::vector<InputActions> &mappings) -> void;

    /// @brief Checks if an action was pressed in this frame
    /// @param action_name The action's name
    /// @return true if is the action was pressed in this frame
    static auto action_just_pressed(std::string_view action_name) -> bool;

    /// @brief Checks if an action is pressed
    /// @param action_name The action's name
    /// @return true if is the action is pressed
    static auto action_pressed(std::string_view action_name) -> bool;

    /// @brief Checks if an action was released in this frame
    /// @param action_name The action's name
    /// @return true if is the action is released in this frame
    static auto action_just_released(std::string_view action_name) -> bool;

    /// @brief Checks if an action is released
    /// @param action_name The action's name
    /// @return true if is the action is released
    static auto action_released(std::string_view action_name) -> bool;

    /// @brief Checks if an action is held, but not pressed in this frame
    /// @param action_name The action's name
    /// @return true if is the action is held, but not pressed in this frame
    static auto action_held(std::string_view action_name) -> bool;

private:
    friend class Window;

    inline static std::array<InputInfo, NUM_PHYSICAL_KEYS>  key_states_;
    inline static std::vector<std::size_t>                  dirty_keys_;

    inline static std::array<InputInfo, NUM_MOUSE_BUTTONS>  mouse_buttons_;
    inline static MouseMouvement                            last_mouse_mvt_;

    struct ActionData
    {
        enum class Type
        {
            KEYBOARD,
            MOUSE
        } type;

        int ordinal;
    };

    inline static std::unordered_map<
        std::string_view, 
        std::vector<ActionData>
    > actions_;

    static auto scancode(PhysicalKey key) -> int;

    static auto update() -> void;
    static auto update_state(InputInfo &state) -> bool;

    static auto glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) -> void;
    static auto glfw_cursor_pos_callback(GLFWwindow *window, double xPos, double yPos) -> void;
    static auto glfw_mouse_btn_callback(GLFWwindow *window, int button, int action, int mods) -> void;
};
    
} // namespace NoctisEngine::Input
