#pragma once
#include <noctis_engine/math/math.hpp>


namespace NoctisEngine
{
    
struct alignas(16) CameraData 
{
    glm::mat4 proj_mat;
    glm::mat4 view_mat;
    glm::vec3 pos;
};

} // namespace NoctisEngine