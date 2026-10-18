#include <noctis_engine/rendering/camera_2d.hpp>

#include <noctis_engine/rendering/buffer_utils.hpp>
#include <noctis_engine/rendering/shader_bindings.hpp>

#include "camera_data.hpp"
#include <noctis_engine/math/mat4x4.hpp>
#include <noctis_engine/math/vec3.hpp>
#include <noctis_engine/math/matrix_math.hpp>


namespace NoctisEngine
{
    
Camera2D::Camera2D(
    const Vec2 &half_extents, 
    const Vec2 &position, 
    float       near, 
    float       far)
    : camera_ubo_{sizeof(CameraData), "camera_ubo", BufferFlag::DYNAMIC_STORAGE_BIT}
    , projection_matrix_{Mat4x4::IDENTITY}
    , pos_{position}
{
    projection_matrix_ = ortho_projection_matrix(
        -half_extents.x, half_extents.x, 
        -half_extents.y, half_extents.y, 
        near, far
    );

    update_buffers();
}

auto Camera2D::translate(const Vec2 &translation) -> void
{
    pos_ += translation;
}

auto Camera2D::set_position(const Vec2 &position) -> void
{
    pos_ = position;
}

auto Camera2D::update_projection_matrix(const Vec2 &new_half_extents) -> void
{
    projection_matrix_[0][0] = 1.0f / new_half_extents.x;
    projection_matrix_[1][1] = 1.0f / new_half_extents.y;
}

auto Camera2D::update_buffers() -> void
{
    camera_ubo_.write(get_cpu_read_view(CameraData{
        .proj_mat = projection_matrix_,
        .view_mat = translate_matrix(Mat4x4::IDENTITY, -Vec3{pos_.x, pos_.y, 0.f}),
        .pos = Vec3{pos_.x, pos_.y, 0.f},
    }), 0);
}

auto Camera2D::use(DrawList &draw_list) -> void
{
    draw_list.bind_buffer_base(
        camera_ubo_, 
        BufferTarget::UNIFORM_BUFFER, 
        ShaderBindings::CAMERA_DATA_UBO
    );
}

auto Camera2D::delete_buffer() -> void
{
    camera_ubo_.delete_gpu();
}
    
} // namespace NoctisEngine