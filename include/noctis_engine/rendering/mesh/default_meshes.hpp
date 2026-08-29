#pragma once
#include "mesh_info.hpp"
#include "mesh_view.hpp"
#include "../vertex.hpp"

namespace NoctisEngine::Rendering::DefaultMeshes
{

/// @brief A simple flat square
inline static const MeshData QUAD_MESH
{
    {
        Vertex{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1)},
        Vertex{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1)},
        Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0)},
        Vertex{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0)},
    },
    {0, 1, 2, 2, 3, 0}
};

/// @brief The quad's mesh view
inline static const MeshView QUAD_MESH_VIEW
{
    .vertices_offset = 0,
    .vertices_count = 4,
    .indices_offset = 0,
    .indices_count = 6
};

} // namespace NoctisEngine::Rendering::DefaultMeshes