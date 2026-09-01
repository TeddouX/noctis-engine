#pragma once
#include "uniform_info.hpp"
#include "mesh/mesh_view.hpp"
#include "../math/color.hpp"


namespace NoctisEngine
{

/// @internal
/// @brief Describes a draw command's type
enum class DrawCommandType
{
    BIND_TEXTURE,
    BIND_PROGRAM,
    BIND_VAO,
    BIND_BUFFER,
    BIND_BUFFER_BASE,
    SET_UNIFORM,
    DRAW_MESH,
    CLEAR,
    DRAW_LINES,
    DRAW_TRIANGLES,
};

/// @internal
/// @brief The draw command that binds a texture
struct BindTextureCmd
{
    std::uint32_t       tex;
    std::uint32_t       bind_point;
    std::string_view    tex_name;
};

/// @internal
/// @brief The draw command that binds a program
struct BindProgramCmd
{
    std::uint32_t prog;
};

/// @internal
/// @brief The draw command that binds a VAO
struct BindVAOCmd
{
    std::uint32_t vao;
};

/// @internal
/// @brief The draw command that sets a uniform of a previously bound shader
struct SetUniformCmd
{
    UniformInfo info;
};

/// @internal
/// @brief The draw command that draws a mesh
struct DrawMeshCmd
{
    MeshView mesh_view;
    glm::mat4 model_mat;
};

/// @internal
/// @brief The draw command that binds a buffer
struct BindBufferCmd
{
    std::uint32_t buffer;
    std::uint32_t target;
};

/// @internal
/// @brief The draw command that binds a buffer to a binding point
struct BindBufferBaseCmd
{
    std::uint32_t buffer;
    std::uint32_t target;
    std::uint32_t bind_point;
};

/// @internal
/// @brief The draw command that clears the screen
struct ClearCmd
{
    Color color;
    double depth;
    bool clear_color;
    bool clear_depth;
};

/// @internal
/// @brief The draw command that draws triangles
struct DrawTrianglesCommand
{
    std::size_t first;
    std::size_t count;
};

/// @internal
/// @brief The draw command that draws lines
struct DrawLinesCommand
{
    std::size_t first;
    std::size_t count;
};

} // namespace NoctisEngine
