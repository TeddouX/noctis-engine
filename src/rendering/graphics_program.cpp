#include <noctis_engine/rendering/graphics_program.hpp>

#include <GL/gl.h>


namespace NoctisEngine::Rendering
{
    
GraphicsProgram::GraphicsProgram(const std::vector<Shader> &shaders, std::string_view name)
    : valid_{true}
{
    bool has_vert_shader = false;
    bool has_frag_shader = false;
    bool has_geo_shader = false;

    handle_ = glCreateProgram();
    glObjectLabel(GL_PROGRAM, handle_, -1, name.data());

    for (const auto &shader : shaders)
    {
        switch (shader.type())
        {
            case ShaderType::COMPUTE:
                RENDERING_LOGGER.error(
                    "Tried to attach shader \"{}\" which is a compute shader in a graphics program \"{}\"", 
                    shader.name(),
                    name
                );
                valid_ = false;
                return;

            case ShaderType::FRAGMENT:
                if (has_frag_shader)
                {
                    RENDERING_LOGGER.warn("Tried to attach two fragment shaders to program \"{}\"", name);
                    continue;
                }
                has_frag_shader = true;
                break;

            case ShaderType::GEOMETRY:
                if (has_geo_shader)
                {
                    RENDERING_LOGGER.warn("Tried to attach two geometry shaders to program \"{}\"", name);
                    continue;
                }
                has_geo_shader = true;
                break;

            case ShaderType::VERTEX:
                if (has_vert_shader)
                {
                    RENDERING_LOGGER.warn("Tried to attach two vertex shaders to program \"{}\"", name);
                    continue;
                }
                has_vert_shader = true;
                break;
        }

        glAttachShader(handle_, shader.gl_handle());
    }

    if (not has_frag_shader)
    {
        RENDERING_LOGGER.error("Graphics program is missing mandatory fragment shader");
        valid_ = false;
    }

    if (not has_vert_shader)
    {
        RENDERING_LOGGER.error("Graphics program is missing mandatory fragment shader");
        valid_ = false;
    }
}

auto GraphicsProgram::link() const -> bool
{
    if (not valid_)
    {
        RENDERING_LOGGER.error("Tried to link an invalid program");
        return false;
    }

    int success;
    char info_log[1024];

    glLinkProgram(handle_);
    glGetProgramiv(handle_, GL_LINK_STATUS, &success);

    if (!success) 
    {
        glGetProgramInfoLog(handle_, sizeof(info_log), nullptr, info_log);
        RENDERING_LOGGER.error("Program linking failed: {}", info_log);
        
        return false;
    }

    return true;
}

auto GraphicsProgram::detach_shader(const Shader &shader) const -> void
{
    glDetachShader(handle_, shader.gl_handle());
}


auto GraphicsProgram::delete_gpu() const -> void
{
    glDeleteProgram(handle_);
}

auto GraphicsProgram::bind(DrawList &draw_list) const -> void
{
    draw_list.bind_program(handle_);
}

auto GraphicsProgram::gl_handle() const -> std::uint32_t
{
    return handle_;
}

auto GraphicsProgram::valid() const -> bool
{
    return valid_;
}

} // namespace NoctisEngine::Rendering
