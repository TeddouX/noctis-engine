#pragma once
#include "mesh_info.hpp"


/// @brief Default meshes that are automatically loaded by mesh managers
namespace NoctisEngine::DefaultMeshes
{

/// @brief A simple flat square
inline const MeshData QUAD_MESH
{
    {
        Vertex{Vec3( 0.5f,  0.5f, 0.0f), Vec3(0), Vec3(0), Vec2(1, 1)},
        Vertex{Vec3(-0.5f,  0.5f, 0.0f), Vec3(0), Vec3(0), Vec2(0, 1)},
        Vertex{Vec3(-0.5f, -0.5f, 0.0f), Vec3(0), Vec3(0), Vec2(0, 0)},
        Vertex{Vec3( 0.5f, -0.5f, 0.0f), Vec3(0), Vec3(0), Vec2(1, 0)},
    },
    {0, 1, 2, 2, 3, 0}
};

} // namespace NoctisEngine::DefaultMeshes