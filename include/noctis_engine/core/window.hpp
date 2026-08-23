#pragma once 
#include <cstdint>
#include <string>


struct GLFWwindow;


namespace NoctisEngine::Core
{
    
class Window
{
public:
    Window(std::uint32_t width, std::uint32_t height, std::string_view title);
    ~Window();

    auto is_valid() -> bool;

    auto should_close() -> bool;

    auto poll_events() -> void;
    auto swap_buffers() -> void;

private:
    GLFWwindow *glfwWindow_;
};

} // namespace NoctisEngine::Core
