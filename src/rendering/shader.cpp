#include <rendering/shader.hpp>

#include <GL/gl.h>

#include <print>

#include <core/logging.hpp>


namespace NoctisEngine::Rendering
{
    
Shader::Shader(const char *code, const std::string &name) 
{
    vert_shader_ = glCreateShader(GL_VERTEX_SHADER);
    frag_shader_ = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertex_shader_name = name + "_vertex";
    std::string frag_shader_name = name + "_frag";

    glObjectLabel(GL_SHADER, vert_shader_, -1, vertex_shader_name.c_str());
    glObjectLabel(GL_SHADER, frag_shader_, -1, frag_shader_name.c_str());

    const std::string header = std::string(OPENGL_VERSION) + "\n" + 
R"(#extension GL_ARB_gpu_shader_int64 : enable
#extension GL_ARB_shader_draw_parameters : require
#define )";

    const std::string vert_code_str = header + "VERTEX\n" + code;
    const std::string frag_code_str = header + "FRAGMENT\n" + code;

    const char *vert_code = vert_code_str.c_str();
    const char *frag_code = frag_code_str.c_str();
    
    glShaderSource(vert_shader_, 1, &vert_code, nullptr);
    glShaderSource(frag_shader_, 1, &frag_code, nullptr);
    
    programID_ = glCreateProgram();
    glObjectLabel(GL_PROGRAM, programID_, -1, name.c_str());
}

auto Shader::compile() -> bool 
{
    int success;
    char infolog[1024];

    glCompileShader(vert_shader_);
    glGetShaderiv(vert_shader_, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        glGetShaderInfoLog(vert_shader_, sizeof(infolog), nullptr, infolog);
        RENDERING_LOGGER.error("Vertex shader compilation failed: {}", infolog);
        return false;
    }

    glCompileShader(frag_shader_);
    glGetShaderiv(frag_shader_, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        glGetShaderInfoLog(frag_shader_, sizeof(infolog), nullptr, infolog);
        RENDERING_LOGGER.error("Fragment shader compilation failed: {}", infolog);
        return false;
    }

    glAttachShader(programID_, vert_shader_);
    glAttachShader(programID_, frag_shader_);
    glLinkProgram(programID_);
    glGetProgramiv(programID_, GL_LINK_STATUS, &success);

    if (!success) 
    {
        glGetProgramInfoLog(programID_, sizeof(infolog), nullptr, infolog);
        RENDERING_LOGGER.error("Program Linking failed: {}", infolog);
        return false;
    }

    // Cleanup
    glDeleteShader(vert_shader_);
    glDeleteShader(frag_shader_);

    return true;
}

void Shader::use(DrawList &draw_list) 
{
    draw_list.bind_program(programID_);
}

auto Shader::gl_handle() -> std::uint32_t
{
    return programID_;
}


} // namespace NoctisEngine::Rendering