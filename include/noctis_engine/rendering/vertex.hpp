#pragma once
#include "../math/math.hpp"
#include "vertex_attrib.hpp"

namespace NoctisEngine
{
    
/// @brief The default vertex used by the engine
struct Vertex
{
    /// @brief Coordinates in local space
    glm::vec3 pos;
    /// @brief The surface normal
    glm::vec3 normal;
    /// @brief The surface tangent
    glm::vec3 tangent;
    /// @brief The texture coordinates
    glm::vec2 tex_coords;
};

/// @brief This is used to draw debug shapes, it is for example used by the physics system
struct DebugVertex
{
    /// @brief In world space coordinates
    glm::vec3 pos;
    /// @brief In RGB format
    glm::vec3 color;
};

/// @brief Vertex attributes for the default vertex
inline static std::vector<VertexAttribute> DEFAULT_VERTEX_ATTRIBUTES = {
    // Pos
    VertexAttribute{
        .component_type = VertexAttribute::ComponentType::FLOAT,
        .num_components = 3,
    },
    // Normal
    VertexAttribute{
        .component_type = VertexAttribute::ComponentType::FLOAT,
        .num_components = 3,
    },
    // Tangent
    VertexAttribute{
        .component_type = VertexAttribute::ComponentType::FLOAT,
        .num_components = 3,
    },
    // Texture coordinates
    VertexAttribute{
        .component_type = VertexAttribute::ComponentType::FLOAT,
        .num_components = 2,
    },
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

} // namespace NoctisEngine