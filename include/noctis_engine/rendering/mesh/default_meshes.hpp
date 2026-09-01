#pragma once
#include "mesh_info.hpp"
#include "mesh_view.hpp"
#include "../vertex.hpp"


/// @brief Default meshes that are automatically loaded by mesh managers
namespace NoctisEngine::DefaultMeshes
{

/// @brief A simple flat square
inline const MeshData QUAD_MESH
{
    {
        Vertex{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1)},
        Vertex{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1)},
        Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0)},
        Vertex{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0)},
    },
    {0, 1, 2, 2, 3, 0}
};

} // namespace NoctisEngine::DefaultMeshes