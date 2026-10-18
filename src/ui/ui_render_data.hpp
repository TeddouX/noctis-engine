#pragma once
#include <vector>

#include <noctis_engine/rendering/mesh/mesh_view.hpp>
#include <noctis_engine/rendering/vertex_attrib.hpp>
#include <noctis_engine/math/vec2.hpp>
#include <noctis_engine/math/vec3.hpp>


namespace NoctisEngine
{
    
struct UIVertex
{
    Vec3 pos;
};

static std::vector<UIVertex> UI_QUAD_VERTICES = 
{
    UIVertex{Vec3( 0.5f,  0.5f, 0.0f)},
    UIVertex{Vec3(-0.5f,  0.5f, 0.0f)},
    UIVertex{Vec3(-0.5f, -0.5f, 0.0f)},
    UIVertex{Vec3( 0.5f, -0.5f, 0.0f)},
};

struct ObjectData
{
    /// @brief x, y [0, 1] position in screen space coordinates, z depth 
    Vec3 pos_data;
};

static const MeshView UI_QUAD_MESH_VIEW = MeshView
{
    .vertices_offset = 0,
    .vertices_count = 4,
    .indices_offset = 0,
    .indices_count = 6,
};

} // namespace NoctisEngine
