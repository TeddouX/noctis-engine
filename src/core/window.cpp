#include <noctis_engine/core/window.hpp>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <noctis_engine/definitions.hpp>
#include <noctis_engine/core/logging.hpp>
#include <noctis_engine/core/input/input_handler.hpp>


namespace NoctisEngine::Core
{
 
Window::Window(std::uint32_t width, std::uint32_t height, std::string_view title)
    : glfw_window_{nullptr}
{
#ifdef NCENG_LINUX
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif

    glfwSetErrorCallback([](int error, const char* description) 
    {
        CORE_LOGGER.error("GLFW error {}: {}", error, description);
    });

    if (!glfwInit())
    {
        CORE_LOGGER.error("Failed to initialize GLFW");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    glfw_window_ = glfwCreateWindow(
        static_cast<int>(width), 
        static_cast<int>(height), 
        title.data(), 
        nullptr, nullptr
    );

    if (!glfw_window_)
        return;
    
    CORE_LOGGER.debug("Created window \"{}\"", title);

    glfwSetKeyCallback(glfw_window_, InputHandler::glfw_key_callback);
    glfwSetMouseButtonCallback(glfw_window_, InputHandler::glfw_mouse_btn_callback);
    glfwSetCursorPosCallback(glfw_window_, InputHandler::glfw_cursor_pos_callback);

    glfwSetWindowUserPointer(glfw_window_, this);
    glfwMakeContextCurrent(glfw_window_);

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
    glfwDestroyWindow(glfw_window_);
    CORE_LOGGER.debug("Destroyed window");

    glfwTerminate();
    CORE_LOGGER.debug("Terminated GLFW");
}

auto Window::is_valid() -> bool
{
    return glfw_window_ != nullptr;
}

auto Window::should_close() -> bool
{
    return glfwWindowShouldClose(glfw_window_);   
}

auto Window::poll_events() -> void
{
    InputHandler::update();
    glfwPollEvents();
}

auto Window::swap_buffers() -> void
{
    glfwSwapBuffers(glfw_window_);

    double curr_frame = glfwGetTime();
	delta_time_ = curr_frame - last_frame_;
	last_frame_ = curr_frame;
}

auto Window::time() -> float
{
    return glfwGetTime(); 
}

auto Window::delta_time() -> float
{
    return delta_time_;
}

auto Window::set_vsync(VSyncMethod method) -> void
{
    glfwSwapInterval(static_cast<int>(method));
}
 
} // namespace NoctisEngine::Core
