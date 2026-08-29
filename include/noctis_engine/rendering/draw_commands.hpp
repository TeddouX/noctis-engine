#pragma once
#include "uniform_info.hpp"
#include "mesh/mesh_view.hpp"
#include "../math/color.hpp"


namespace NoctisEngine::Rendering
{
   
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
    
struct BindTextureCmd
{
    std::uint32_t       tex;
    std::uint32_t       bind_point;
    std::string_view    tex_name;
};

struct BindProgramCmd
{
    std::uint32_t prog;
};

struct BindVAOCmd
{
    std::uint32_t vao;
};

struct SetUniformCmd
{
    UniformInfo info;
};

struct DrawMeshCmd
{
    MeshView mesh_view;
    glm::mat4 model_mat;
};

struct BindBufferCmd
{
    std::uint32_t buffer;
    std::uint32_t target;
};

struct BindBufferBaseCmd
{
    std::uint32_t buffer;
    std::uint32_t target;
    std::uint32_t bind_point;
};

struct ClearCmd
{
    Color color;
    double depth;
    bool clear_color;
    bool clear_depth;
};

struct DrawTrianglesCommand
{
    std::size_t first;
    std::size_t count;
};

struct DrawLinesCommand
{
    std::size_t first;
    std::size_t count;
};

} // namespace NoctisEngine::Rendering
