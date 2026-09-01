#include <noctis_engine/core/window.hpp>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <noctis_engine/definitions.hpp>
#include <noctis_engine/core/logging.hpp>
#include <noctis_engine/core/input/input_handler.hpp>


namespace NoctisEngine
{
 
Window::Window(std::uint32_t width, std::uint32_t height, std::string_view title)
    : glfw_window_{nullptr}
    , resize_callback_{nullptr}
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
    glfwSetFramebufferSizeCallback(glfw_window_, framebuffer_resized_callback);

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

auto Window::get_primary_monitor() -> Monitor
{
    return make_nceng_monitor(glfwGetPrimaryMonitor());
}

auto Window::get_all_monitors() -> std::vector<Monitor>
{
    std::vector<Monitor> vec;

    int monitor_count;
    GLFWmonitor **monitors = glfwGetMonitors(&monitor_count);
    for (int i = 0; i < monitor_count; i++)
    {
        GLFWmonitor *monitor = monitors[i];
        vec.push_back(make_nceng_monitor(monitor));
    }

    return vec;
}

auto Window::set_floating() -> void
{
    if (not saved_state_.fullscreen)
        return;

    glfwSetWindowAttrib(glfw_window_, GLFW_DECORATED, GLFW_TRUE);
    glfwSetWindowAttrib(glfw_window_, GLFW_FLOATING, GLFW_FALSE);

    glfwSetWindowMonitor(
        glfw_window_, 
        NULL,
        saved_state_.x, saved_state_.y,
        saved_state_.width, saved_state_.height, 
        0
    );

    saved_state_.fullscreen = false;
}

auto Window::set_fullscreen_borderless(Monitor &monitor, Monitor::VideoMode &mode) -> void
{
    glfwGetWindowPos(glfw_window_, &saved_state_.x, &saved_state_.y);
    glfwGetWindowSize(glfw_window_, &saved_state_.width, &saved_state_.height);

    glfwSetWindowAttrib(glfw_window_, GLFW_DECORATED, GLFW_FALSE);
    glfwSetWindowAttrib(glfw_window_, GLFW_FLOATING, GLFW_TRUE);

    int monitor_x, monitor_y;
    glfwGetMonitorPos(monitor.ptr, &monitor_x, &monitor_y);
    printf("Monitor: %.*s | pos: (%d, %d) | mode: %dx%d@%d\n",
       (int)monitor.name.size(), monitor.name.data(),
       monitor_x, monitor_y, mode.width, mode.height, mode.refresh_rate);

    glfwSetWindowMonitor(
        glfw_window_, 
        NULL, 
        monitor_x, monitor_y,
        mode.width, mode.height,
        mode.refresh_rate
    );

    saved_state_.fullscreen = true;
}

auto Window::set_fullscreen(Monitor &monitor, Monitor::VideoMode &mode) -> void
{
    glfwGetWindowPos(glfw_window_, &saved_state_.x, &saved_state_.y);
    glfwGetWindowSize(glfw_window_, &saved_state_.width, &saved_state_.height);

    glfwSetWindowMonitor(
        glfw_window_, 
        monitor.ptr, 
        0, 0,
        mode.width, mode.height, 
        mode.refresh_rate
    );

    saved_state_.fullscreen = true;
}

auto Window::make_nceng_monitor(GLFWmonitor *glfw_monitor) -> Monitor
{
    Monitor monitor{};
    monitor.name = glfwGetMonitorName(glfw_monitor);
    monitor.ptr = glfw_monitor;

    int mode_count = 0;
    const GLFWvidmode *video_modes = glfwGetVideoModes(glfw_monitor, &mode_count);
    for (int i = 0; i < mode_count; i++)
    {
        const GLFWvidmode &vid_mode = video_modes[i];
        monitor.video_modes.push_back(Monitor::VideoMode{
            .width = vid_mode.width,
            .height = vid_mode.height,
            .refresh_rate = vid_mode.refreshRate,
        });
    }

    const GLFWvidmode *curr_mode = glfwGetVideoMode(glfw_monitor);
    monitor.current_video_mode = Monitor::VideoMode{
        .width = curr_mode->width,
        .height = curr_mode->height,
        .refresh_rate = curr_mode->refreshRate,
    };

    return monitor;
}

auto Window::set_resize_callback(std::function<void (int, int)> callback) -> void
{
    resize_callback_ = callback;
}

auto Window::framebuffer_resized_callback(GLFWwindow* glfw_window, int width, int height) -> void
{
    glViewport(0, 0, width, height);

    Window *window = (Window *)glfwGetWindowUserPointer(glfw_window);
    if (window->resize_callback_)
        window->resize_callback_(width, height);
}

} // namespace NoctisEngine
