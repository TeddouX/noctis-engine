#pragma once
#include "../math/math.hpp"

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

} // namespace NoctisEngine::Rendering