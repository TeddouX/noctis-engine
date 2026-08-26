#include <noctis_engine/rendering/camera_2d.hpp>

#include <noctis_engine/rendering/shader_bindings.hpp>
#include <noctis_engine/rendering/gpu_buffer_utils.hpp>
#include <noctis_engine/math/math.hpp>

#include "camera_data.hpp"


namespace NoctisEngine::Rendering
{
    
Camera2D::Camera2D(float frustumWidth, float frustumHeight, float near, float far)
    : proj_mat_{glm::ortho(0.f, frustumWidth, frustumHeight, 0.f, near, far)}
    , uniform_buffer_{sizeof(CameraData), "camera2D_ubo"}
    , pos_{0}
{
    
}

auto Camera2D::upload_data() -> void 
{
    uniform_buffer_.write(get_cpu_buffer_view(
        CameraData{
            .proj_mat = proj_mat_,
            .view_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-pos_, 0.0f)),
            .pos = glm::vec4{pos_, 0, 0},
        }
    ), 0);
}

auto Camera2D::use(DrawList &draw_list) const -> void
{
    draw_list.bind_buffer_base(
        uniform_buffer_, 
        BufferTarget::UNIFORM_BUFFER, 
        ShaderBindings::CAMERA_DATA_UBO
    );
}

auto Camera2D::set_position(glm::vec2 pos) -> void 
{
    pos_ = pos;
}

auto Camera2D::translate_by(glm::vec2 translation) -> void 
{
    pos_ += translation;
}

    
} // namespace NoctisEngine::Rendering