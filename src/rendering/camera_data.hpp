#pragma once
#include <noctis_engine/math/mat4x4.hpp>
#include <noctis_engine/math/vec3.hpp>


namespace NoctisEngine
{
    
struct alignas(16) CameraData 
{
    Mat4x4  proj_mat;
    Mat4x4  view_mat;
    Vec3    pos;
};

} // namespace NoctisEngine