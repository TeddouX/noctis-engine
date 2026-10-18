#pragma once
#include <vector>

#include "../math/vec3.hpp"
#include "../math/vec2.hpp"
#include "vertex_attrib.hpp"


namespace NoctisEngine
{
    
/// @brief The default vertex used by the engine
struct Vertex
{
    /// @brief Coordinates in local space
    Vec3 pos;
    /// @brief The surface normal
    Vec3 normal{0};
    /// @brief The surface tangent
    Vec3 tangent{0};
    /// @brief The texture coordinates
    Vec2 tex_coords{0};
};

/// @brief This is used to draw debug shapes, it is for example used by the physics system
struct DebugVertex
{
    /// @brief In world space coordinates
    Vec3 pos;
    /// @brief In RGB format
    Vec3 color;
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