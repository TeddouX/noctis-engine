#pragma once
#include <cstdint>
#include <vector>

#include "shader.hpp"


namespace NoctisEngine::Rendering
{


class GraphicsProgram
{
public:
    /// @brief Creates a graphics program on the GPU and attaches shaders to it
    /// @param shaders The shaders that should be linked to this program
    /// @warning Compute shaders are not accepted by this program. 
    /// Use the dedicated ComputeProgram class instead.
    GraphicsProgram(const std::vector<Shader> &shaders);
    
    ~GraphicsProgram() = default;

    /// @brief Links this program's shaders to it
    /// @return `true` on success `false` on error
    auto link() -> bool;

    /// @brief Deletes this program on the GPU
    auto delete_gpu() -> void;

    /// @return This shader's OpenGL handle
    auto gl_handle() -> std::uint32_t;

private:
    std::uint32_t handle_;
};

} // namespace NoctisEngine::Rendering
