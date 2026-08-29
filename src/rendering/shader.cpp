#include <noctis_engine/rendering/shader.hpp>

#include <GL/gl.h>
#include <string>

#include <noctis_engine/core/logging.hpp>
#include <noctis_engine/noctis_engine.hpp>


namespace NoctisEngine::Rendering
{
    
constexpr std::string_view SHADER_HEADER = 
R"(#extension GL_ARB_gpu_shader_int64 : enable
#extension GL_ARB_shader_draw_parameters : require
)";

Shader::Shader(ShaderType type, std::string_view code, std::string_view name)
    : name_{name}
{
    handle_ = glCreateShader(static_cast<GLenum>(type));
    glObjectLabel(GL_SHADER, handle_, -1, name.data());

    std::string final_code = std::string(OPENGL_VERSION) 
        + "\n" 
        + std::string(SHADER_HEADER) 
        + std::string(code);

    const char *final_code_data = final_code.data();
    glShaderSource(handle_, 1, &final_code_data, nullptr);
}
    
auto Shader::compile() -> bool
{
    int success;
    char info_log[1024];

    glCompileShader(handle_);
    glGetShaderiv(handle_, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        glGetShaderInfoLog(handle_, sizeof(info_log), nullptr, info_log);
        RENDERING_LOGGER.error("\"{}\" compilation failed: {}", name_, info_log);

        return false;
    }

    return true;
}

auto Shader::gl_handle() -> std::uint32_t
{
    return handle_;
}

auto Shader::delete_gpu() -> void
{
    glDeleteShader(handle_);
}

} // namespace NoctisEngine::Rendering