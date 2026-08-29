#pragma once
#include <cstdint>
#include <vector>

#include "shader.hpp"
#include "draw_list.hpp"


namespace NoctisEngine::Rendering
{

/// @brief Describes a program that links shaders used for rendering
class GraphicsProgram
{
public:
    /// @brief Default constructor creates an invalid graphics program
    GraphicsProgram() = default;

    /// @brief Creates a graphics program on the GPU and attaches shaders to it
    /// @param shaders The shaders that should be linked to this program
    /// @param name This program's name
    /// @warning Compute shaders are not accepted by this program. 
    /// Use the dedicated ComputeProgram class instead.
    GraphicsProgram(const std::vector<Shader> &shaders, std::string_view name);
    
    ~GraphicsProgram() = default;

    /// @brief A helper for graphics program creation
    /// @param vert_code The vertex shader code
    /// @param frag_code The fragment shader code
    /// @param name The program's name, also used for the shaders' names
    /// @return A valid graphics program on success
    static auto create_helper(
        std::string_view vert_code, 
        std::string_view frag_code, 
        std::string_view name
    ) -> GraphicsProgram;

    /// @brief Links this program's shaders to it
    /// @return `true` on success `false` on error
    auto link() const -> bool;

    /// @brief Detaches a shader from this program
    /// @param shader The shader that should be detached
    /// @warning If the shader wasn't attached to this program in the first place, 
    /// OpenGL will generate an error
    auto detach_shader(const Shader &shader) const -> void;

    /// @brief Deletes this program on the GPU
    auto delete_gpu() const -> void;

    /// @brief Binds this graphics program to a draw list
    /// @param draw_list The draw list that it should be bound to
    auto bind(DrawList &draw_list) const -> void;

    /// @return This shader's OpenGL handle
    auto gl_handle() const -> std::uint32_t;

    /// @return `true` if the program is valid, meaning it has the necessary shaders, 
    /// `false` otherwise 
    auto valid() const -> bool;

private:
    bool            valid_{false};
    std::uint32_t   handle_;
};

} // namespace NoctisEngine::Rendering
