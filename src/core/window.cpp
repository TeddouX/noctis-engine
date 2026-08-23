#include <core/window.hpp>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <noctis_engine.hpp>


namespace NoctisEngine
{
 
Window::Window(std::uint32_t width, std::uint32_t height, std::string_view title)
    : glfwWindow_{nullptr}
{
    if (!glfwInit())
    {
        CORE_LOGGER.error("Failed to initialize GLFW");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    glfwSetErrorCallback([](int error, const char* description) {
        CORE_LOGGER.error("GLFW error {}: {}", error, description);
    });

    glfwWindow_ = glfwCreateWindow(
        static_cast<int>(width), 
        static_cast<int>(height), 
        title.data(), 
        nullptr, nullptr
    );

    if (!glfwWindow_)
        return;
    
    CORE_LOGGER.debug("Created window \"{}\"", title);

    glfwSetWindowUserPointer(glfwWindow_, this);
    glfwMakeContextCurrent(glfwWindow_);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        CORE_LOGGER.error("Failed to load OpenGL functions");

        glfwTerminate();
        return;
    }

    CORE_LOGGER.debug("Initialized OpenGL context for window \"{}\"", title);
}

Window::~Window()
{
    glfwDestroyWindow(glfwWindow_);
    CORE_LOGGER.debug("Destroyed window");

    glfwTerminate();
    CORE_LOGGER.debug("Terminated GLFW");
}

auto Window::is_valid() -> bool
{
    return glfwWindow_ != nullptr;
}

auto Window::should_close() -> bool
{
    return glfwWindowShouldClose(glfwWindow_);   
}

auto Window::poll_events() -> void
{
    glfwPollEvents();
}

auto Window::swap_buffers() -> void
{
    glfwSwapBuffers(glfwWindow_);
}
 
} // namespace NoctisEngine
