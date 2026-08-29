#pragma once
#include <cstddef>

namespace NoctisEngine::Rendering
{

/// @brief Describes a mesh that is stored on the GPU
struct MeshView 
{
    /// @brief The first vertex
    std::size_t vertices_offset;
    
    /// @brief The number of vertices 
    std::size_t vertices_count;

    /// @brief The first index
    std::size_t indices_offset;

    /// @brief The number of indices
    std::size_t indices_count;
};

} // namespace NoctisEngine::Rendering