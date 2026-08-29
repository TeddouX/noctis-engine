#pragma once 
#include <cstdint>
#include <string>


struct GLFWwindow;


namespace NoctisEngine::Core
{
    
/// @brief Describes all vertical sync methods supported by Window
enum class VSyncMethod : int 
{
    /// @brief No vertical sync
    NO_VSYNC = 0,

    /// @brief Vertical sync set to the screen's refresh rate
    VSYNC = 1,

    /// @brief Vertical sync set to half of the screen's refresh rate
    HALF_REFRESH_RATE = 2,
};

/// @brief This class is a wrappter over a GLFW window 
class Window
{
public:
    /// @brief Creates a window
    /// @param width The window's width
    /// @param height The window's height
    /// @param title The window's title
    Window(std::uint32_t width, std::uint32_t height, std::string_view title);
    ~Window();

    /// @return true if the window was created successfully 
    auto is_valid() -> bool;

    /// @return true if the user requested the window to close 
    auto should_close() -> bool;

    /// @brief Polls GLFW events (input, window events, ...)
    auto poll_events() -> void;

    /// @brief Presents elements drawn by OpenGL
    auto swap_buffers() -> void;

    /// @brief Returns time since window creation
    auto time() -> float;

    /// @brief Returns the last frame's delta time
    auto delta_time() -> float;

    /// @brief Sets the vsync method for this window
    /// @param method The vsync method
    auto set_vsync(VSyncMethod method) -> void;

private:
    GLFWwindow *glfw_window_;

    double delta_time_;
    double last_frame_;
};

} // namespace NoctisEngine::Core
