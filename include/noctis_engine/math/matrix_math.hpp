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
        {  c * scale.x,     s * scale.x,    0.0f, 0.0f },
        { -s * scale.y,     c * scale.y,    0.0f, 0.0f },
        {  0.0f,            0.0f,           1.0f, 0.0f },
        { position.x,       position.y,     0.0f, 1.0f }
    };
}

/// @brief Translates a matrix
constexpr inline auto translate_matrix(const Mat4x4 &mat, const Vec3 &translation) -> Mat4x4
{
    Mat4x4 copy = mat;
    copy[0][3] += translation.x;
    copy[1][3] += translation.y;
    copy[2][3] += translation.z;
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

    mat[0][0] = 2 / rminl;
    mat[0][3] = -(right + left) / rminl;
    mat[1][1] = 2 / tminb;
    mat[1][3] = -(top + bottom) / tminb;
    mat[2][2] = -2 / fminn;
    mat[2][2] = -(far + near) / fminn;

    return mat;
}

}
