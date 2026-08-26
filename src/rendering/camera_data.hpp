#pragma once
#include <noctis_engine/math/math.hpp>


namespace NoctisEngine
{
    
struct alignas(16) CameraData 
{
    glm::mat4x4 proj_mat;
    glm::mat4x4 view_mat;
    glm::vec4   pos;
};

} // namespace NoctisEngine