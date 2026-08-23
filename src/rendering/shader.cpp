#include <rendering/shader.hpp>

#include <GL/gl.h>

#include <print>

#include <core/logging.hpp>


namespace NoctisEngine::Rendering
{
    
Shader::Shader(const std::string &code, const std::string &name) 
{
    vert_shader_ = glCreateShader(GL_VERTEX_SHADER);
    frag_shader_ = glCreateShader(GL_FRAGMENT_SHADER);

    glObjectLabel(GL_SHADER, vert_shader_, -1, name.c_str());
    glObjectLabel(GL_SHADER, frag_shader_, -1, name.c_str());

    const std::string header = std::string(OPENGL_VERSION) + "\n" + 
R"(#extension GL_ARB_gpu_shader_int64 : enable
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_shader_draw_parameters : require
#define )";

    const std::string vertCodeStr = header + "VERTEX\n" + code;
    const std::string fragCodeStr = header + "FRAGMENT\n" + code;

    const char* vertCode = vertCodeStr.c_str();
    const char* fragCode = fragCodeStr.c_str();
    
    glShaderSource(vert_shader_, 1, &vertCode, nullptr);
    glShaderSource(frag_shader_, 1, &fragCode, nullptr);
    
    programID_ = glCreateProgram();
    glObjectLabel(GL_PROGRAM, programID_, -1, name.c_str());
}

auto Shader::compile() -> void 
{
    int success;
    char infolog[1024];

    glCompileShader(vert_shader_);
    glGetShaderiv(vert_shader_, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        glGetShaderInfoLog(vert_shader_, sizeof(infolog), nullptr, infolog);
        RENDERING_LOGGER.error("Vertex shader compilation failed: {}", infolog);
        return;
    }

    glCompileShader(frag_shader_);
    glGetShaderiv(frag_shader_, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        glGetShaderInfoLog(frag_shader_, sizeof(infolog), nullptr, infolog);
        RENDERING_LOGGER.error("Fragment shader compilation failed: {}", infolog);
        return;
    }

    glAttachShader(programID_, vert_shader_);
    glAttachShader(programID_, frag_shader_);
    glLinkProgram(programID_);
    glGetProgramiv(programID_, GL_LINK_STATUS, &success);

    if (!success) 
    {
        glGetProgramInfoLog(programID_, sizeof(infolog), nullptr, infolog);
        RENDERING_LOGGER.error("Program Linking failed: {}", infolog);
        return;
    }

    // Cleanup
    glDeleteShader(vert_shader_);
    glDeleteShader(frag_shader_);
}

void Shader::bind() 
{
    glUseProgram(programID_);
}

auto Shader::set_uniform(const UniformInfo &info) const -> bool 
{
    int loc = glGetUniformLocation(programID_, info.name.c_str());
    
    if (loc < 0) 
    {
        RENDERING_LOGGER.warn("Uniform {} couldn't be found", info.name);
        return false;
    }

    switch (info.type) 
    {
        case UniformType::BOOL:  glUniform1i(loc, std::get<bool>(info.val)); break;
        case UniformType::INT:   glUniform1i(loc, std::get<int>(info.val)); break;
        case UniformType::FLOAT: glUniform1f(loc, std::get<float>(info.val)); break;
    }

    return true;
}

} // namespace NoctisEngine::Rendering