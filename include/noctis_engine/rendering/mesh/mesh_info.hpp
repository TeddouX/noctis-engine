#pragma once
#include <vector>
#include <string_view>

#include "../../definitions.hpp"
#include "../vertex.hpp"

namespace NoctisEngine
{

/// @brief Describes a mesh
struct MeshData 
{
    /// @brief The meshe's vertices
    std::vector<Vertex> vertices;

    /// @brief The meshe's indices
    std::vector<std::uint32_t> indices;
};

} // namespace NoctisEngine