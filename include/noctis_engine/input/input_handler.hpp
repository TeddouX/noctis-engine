#pragma once
#include "keyboard.hpp"
#include "mouse.hpp"


struct GLFWwindow;


/// @brief This namespace containes everything implemented by the engine for input handling
namespace NoctisEngine::Input
{

/// @brief This is the main class of the engine's input handling
class Handler
{
public:
    Handler() = delete;

    /// @brief Checks if a phyiscal key is either pressed or held
    /// @param key The phyiscal key that should be checked
    /// @return true if is the phyiscal key is pressed or held
    static auto is_key_down(PhysicalKey key) -> bool;

    /// @brief Checks if a phyiscal key is neither pressed nor held
    /// @param key The phyiscal key that should be checked
    /// @return true if is the phyiscal key is neither pressed nor held
    static auto is_key_up(PhysicalKey key) -> bool;

    /// @brief Checks if a phyiscal key is held
    /// @param key The phyiscal key that should be checked
    /// @return true if is the phyiscal key is held
    static auto is_key_held(PhysicalKey key) -> bool;

    /// @brief Checks if a phyiscal key is released
    /// @param key The phyiscal key that should be checked
    /// @return true if is the phyiscal key is released
    static auto is_key_released(PhysicalKey key) -> bool;

    /// @brief Checks if a phyiscal key is pressed
    /// @param key The phyiscal key that should be checked
    /// @return true if is the phyiscal key is pressed
    static auto is_key_pressed(PhysicalKey key) -> bool;

    /// @brief Gets a pressed key's modifiers
    /// @param key The physical key that should be checked
    /// @return None if the key isn't pressed or wasn't pressed with any modifiers, 
    /// the modifiers otherwise
    static auto get_key_modifiers(PhysicalKey key) -> Modifier;

    /// @brief Gets this frame's mouse movement
    /// @return This frame's mouse movement
    static auto get_mouse_mouvement() -> MouseMouvement;

    /// @brief Checks if a mouse button is down
    /// @param key The mouse button that should be checked
    /// @return true if is the mouse button is down
    static auto is_mouse_button_down(MouseButton mb) -> bool;

private:
    friend class Window;

    struct InputState 
    {
        enum class State 
        { 
            UP,
            PRESSED,
            HELD,
            RELEASED 
        } state;

        Modifier mods;
    };

    inline static std::array<InputState, NUM_KEYS> key_states_;
    inline static std::vector<size_t> dirty_keys_;

    inline static std::array<InputState, NUM_MOUSE_BUTTONS> mouse_buttons_;
    inline static MouseMouvement last_mouse_mvt_;

    static auto update() -> void;
    static auto check_key(PhysicalKey key) -> bool;
    static auto update_state(InputState &state) -> void;

    static auto glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) -> void;
    static auto glfw_cursor_pos_callback(GLFWwindow *window, double xPos, double yPos) -> void;
    static auto glfw_mouse_btn_callback(GLFWwindow *window, int button, int action, int mods) -> void;
};
    
} // namespace NoctisEngine::Input
