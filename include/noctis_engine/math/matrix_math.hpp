#pragma once
#include "vec2.hpp"
#include "vec3.hpp"
#include "mat4x4.hpp"

namespace NoctisEngine 
{

/// @brief Creates a model matrix
/// @param position Model's position
/// @param scale Model's scale
/// @param rotation Model's rotation, in radians
constexpr inline auto model_matrix_2d(const Vec2 &position, const Vec2 &scale, float rotation) -> Mat4x4
{
    const float c = std::cos(rotation);
    const float s = std::sin(rotation);

    return Mat4x4{
        {  c * scale.x,  s * scale.x, 0.0f, 0.0f },
        { -s * scale.y,  c * scale.y, 0.0f, 0.0f },
        {  0.0f,         0.0f,        1.0f, 0.0f },
        {  position.x,   position.y,  0.0f, 1.0f }
    };
}

/// @brief Translates a matrix
constexpr inline auto translate_matrix(const Mat4x4 &mat, const Vec3 &translation) -> Mat4x4
{
    Mat4x4 copy = mat;

    copy[3][0] += translation.x;
    copy[3][1] += translation.y;
    copy[3][2] += translation.z;

    return copy;
}

/// @brief Scales a matrix
constexpr inline auto scale_matrix(const Mat4x4 &mat, const Vec3 &scale) -> Mat4x4
{
    Mat4x4 copy = mat;

    copy[0] *= scale.x;
    copy[1] *= scale.y;
    copy[2] *= scale.z;
    
    return copy;
}

/// @brief Creates an orthographics projection matrix
constexpr inline auto ortho_projection_matrix(
    float left, float right, 
    float bottom, float top, 
    float near, float far) -> Mat4x4
{
    Mat4x4 mat = Mat4x4::IDENTITY;

    float rminl = right - left;
    float tminb = top - bottom;
    float fminn = far - near;

    mat[0][0] =  2.0f / rminl;
    mat[1][1] =  2.0f / tminb;
    mat[2][2] = -2.0f / fminn;

    mat[3][0] = -(right + left) / rminl;
    mat[3][1] = -(top + bottom) / tminb;
    mat[3][2] = -(far + near) / fminn;

    return mat;
}

}
