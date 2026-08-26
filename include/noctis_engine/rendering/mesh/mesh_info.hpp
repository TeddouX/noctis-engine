#pragma once
#include <vector>
#include <string_view>

#include "../../noctis_engine.hpp"
#include "../vertex.hpp"

namespace NoctisEngine::Rendering
{

struct MeshInfo 
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    constexpr MeshInfo(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
        : vertices(vertices)
        , indices(indices)
    {}
};

} // namespace NoctisEngine::Rendering