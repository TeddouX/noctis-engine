#include <noctis_engine/rendering/camera_2d.hpp>

#include <noctis_engine/rendering/buffer_utils.hpp>
#include <noctis_engine/rendering/shader_bindings.hpp>

#include "camera_data.hpp"


namespace NoctisEngine::Rendering
{
    
Camera2D::Camera2D(
    const glm::vec2    &half_extents, 
    const glm::vec2    &position, 
    float               near, 
    float               far)
    : camera_ubo_{sizeof(CameraData), "camera_ubo", BufferFlag::DYNAMIC_STORAGE_BIT}
    , projection_matrix_{1}
    , pos_{position}
{
    projection_matrix_ = glm::ortho(
        -half_extents.x, half_extents.x, 
        half_extents.y, -half_extents.y, 
        near, far
    );

    update_buffers();
}

auto Camera2D::translate(const glm::vec2 &translation) -> void
{
    pos_ += translation;
}

auto Camera2D::set_position(const glm::vec2 &position) -> void
{
    pos_ = position;
}

auto Camera2D::update_projection_matrix(const glm::vec2 &new_half_extents) -> void
{
    projection_matrix_[0][0] = 1.0f / new_half_extents.x;
    projection_matrix_[1][1] = -1.0f / new_half_extents.y;
}

auto Camera2D::update_buffers() -> void
{
    camera_ubo_.write(get_cpu_read_view(CameraData{
        .proj_mat = projection_matrix_,
        .view_mat = glm::translate(glm::mat4{1}, -glm::vec3{pos_, 0}),
        .pos = glm::vec3{pos_, 0},
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
    
} // namespace NoctisEngine::Rendering