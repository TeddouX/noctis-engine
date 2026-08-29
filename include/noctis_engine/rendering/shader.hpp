#pragma once
#include <cstdint>
#include <string>


namespace NoctisEngine::Rendering
{

/// @brief Represents different shader types
enum class ShaderType
{
    /// @brief Runs once per vertex. Controls where vertices are draw on the screen.
    /// @important This shader is mandatory for draw calls
    VERTEX = 0x8B31,

    /// @brief Runs once per fragment (per pixel that covers a meshe's primitives)
    /// Computes the final color output
    /// @important This shader is mandatory for draw calls
    FRAGMENT = 0x8B30,

    /// @brief Runs once per primitive (point, line, or triangle) rather than per vertex. 
    /// Can emit zero, one, or many primitives from each input primitive. Not mandatory for draw calls
    GEOMETRY = 0x8DD9,

    /// @brief Not part of the drawing pipeline at all. Runs arbitrary parallel work on the GPU.
    /// This shader should be used in a ComputeProgram.
    COMPUTE = 0x91B9, 
};

/// @brief This class represents a shader on the GPU. 
/// Must be linked with other necessary shaders using a shader program
class Shader
{
public:
    /// @brief Default constructor
    Shader() = default;

    /// @brief Creates a shader
    /// @param type The shader's type
    /// @param code The shader's code, null terminated
    /// @param name The shader's name, null terminated
    Shader(ShaderType type, std::string_view code, std::string_view name);
    
    ~Shader() = default;

    /// @return true on success, false on error 
    auto compile() const -> bool;

    /// @return The OpenGL handle to this shader 
    auto gl_handle() const -> std::uint32_t;

    /// @return This shader's type 
    auto type() const -> ShaderType;

    /// @return This shader's name 
    auto name() const -> const std::string &;

    /// @brief Deletes this shader from the GPU. If the shader is attached to a program 
    /// the deletion will be queued until the shader is detached from the program.
    auto delete_gpu() const -> void;

private:
    std::uint32_t   handle_;
    std::string     name_;
    ShaderType      type_;
};

} // namespace NoctisEngine::Rendering