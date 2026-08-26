#pragma once
#include <cstddef>

namespace NoctisEngine::Rendering
{
    
struct MeshView 
{
    std::size_t vertices_offset;
    std::size_t vertices_count;
    std::size_t indices_offset;
    std::size_t indices_count;
};

} // namespace NoctisEngine::Rendering