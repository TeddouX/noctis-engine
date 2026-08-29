#pragma once
#include "../math/math.hpp"
#include "vertex_attrib.hpp"

namespace NoctisEngine::Rendering
{
    
struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 tex_coords;

    constexpr Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec3 tangent, glm::vec2 tex_coords)
        : pos(pos), normal(normal), tangent(tangent), tex_coords(tex_coords) {}
};

/// @brief This is used to draw debug shapes, it is for example used by the physics system
struct DebugVertex
{
    /// @brief In world space coordinates
    glm::vec3 pos;
    /// @brief In RGB format
    glm::vec3 color;
};

/// @brief Vertex attributes for the debug vertex
inline static std::vector<VertexAttribute> DEBUG_VERTEX_ATTRIBUTES = {
    // Pos
    VertexAttribute{
        .component_type = VertexAttribute::ComponentType::FLOAT,
        .num_components = 3,
    },
    // Color
    VertexAttribute{
        .component_type = VertexAttribute::ComponentType::FLOAT,
        .num_components = 3,
    },
};

} // namespace NoctisEngine::Rendering