#pragma once 
#include <cstdint>
#include <string>
#include <vector>
#include <functional>


struct GLFWwindow;
struct GLFWmonitor;


namespace NoctisEngine
{
    
/// @brief Describes all vertical sync methodsn, used in Window::set_vsync()
enum class VSyncMethod : int 
{
    /// @brief No vertical sync
    NO_VSYNC = 0,

    /// @brief Vertical sync set to the screen's refresh rate
    VSYNC = 1,

    /// @brief Vertical sync set to half of the screen's refresh rate
    HALF_REFRESH_RATE = 2,
};

/// @brief This represents a user's monitor
struct Monitor
{
    struct VideoMode
    {
        /// @brief The monitor's width, in pixels
        int width;
        /// @brief The monitor's heigh , in pixels
        int height;
        
        /// @brief This mode's refresh rate
        int refresh_rate;
    };

    /// @brief The monitor's name
    std::string_view name;

    /// @brief All this monitor's supported video modes
    std::vector<VideoMode> video_modes;

    /// @brief This monitor's current video mode
    VideoMode current_video_mode;

private:
    friend class Window;

    GLFWmonitor *ptr;
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

    /// @brief Polls GLFW events (input, window events, ...) and updates the InputHandler
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

    /// @brief Gets the user's primary monitor
    auto get_primary_monitor() -> Monitor;

    /// @brief Gets the all the user's monitors
    auto get_all_monitors() -> std::vector<Monitor>;

    /// @brief Sets the window to be a floating window
    auto set_floating() -> void;

    /// @brief Sets the window to be fullscreen borderless
    /// @param monitor The monitor the window should be attached to
    /// @param mode The mode that should be used
    /// @warning This function is not properly implemented for some OSes and linux desktop envs
    auto set_fullscreen_borderless(Monitor &monitor, Monitor::VideoMode &mode) -> void;

    /// @brief Sets the window to be fullscreen
    /// @param monitor The monitor the window should be attached to
    /// @param mode The mode that should be used
    auto set_fullscreen(Monitor &monitor, Monitor::VideoMode &mode) -> void;

    /// @brief Sets the callback for when the window gets resized, useful for 
    /// resizing a camera's projection matrix
    /// @param callback The callback, param 1: new_width, param 2: new_height
    auto set_resize_callback(std::function<void (int, int)> callback) -> void;

private:
    GLFWwindow *glfw_window_;

    double delta_time_;
    double last_frame_;

    std::function<void (int, int)> resize_callback_;

    struct State
    {
        int x, y, width, height;
        bool fullscreen = false;
    } saved_state_;

    auto make_nceng_monitor(GLFWmonitor *glfw_monitor) -> Monitor;

    static auto framebuffer_resized_callback(GLFWwindow* glfw_window, int width, int height) -> void;
};

} // namespace NoctisEngine
