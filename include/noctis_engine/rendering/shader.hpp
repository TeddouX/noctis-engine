#pragma once


namespace NoctisEngine::Rendering
{

/// @brief Represents different shader types
enum class ShaderType
{
    /// @brief Controls where vertices are draw on the screen
    VERTEX = 0x8B31,

    /// @brief Controls the color of the pixels of your meshes
    FRAGMENT = 0x8B30
};

/// @brief This class represents a shader on the GPU. 
/// Must be linked with other necessary shaders using a shader program
class Shader
{
public:
    /// @brief Creates a shader and compiles it
    /// @param code The shader's code
    /// @param name The shader's name
    Shader(std::string_view code, std::string_view name);
    
    ~Shader() = default;

    /// @return `true` on success, `false` on error 
    auto compile() -> bool;

    /// @return The OpenGL handle to this shader 
    auto gl_handle() -> std::uint32_t;

    /// @brief Deletes this shader from the GPU, call this after 
    /// it was linked in a program to free up memory
    auto delete_gpu() -> void;

private:
};

} // namespace NoctisEngine::Rendering