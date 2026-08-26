#pragma once
#include <glm/mat4x4.hpp>

#include "gpu_buffer.hpp"
#include "draw_list.hpp"


namespace NoctisEngine::Rendering
{
 
class Camera2D 
{
public:
    Camera2D(float frustumWidth, float frustumHeight, float near = -1000.f, float far = 1000.f);
    ~Camera2D() = default;

    auto upload_data() -> void;
    auto use(DrawList &draw_list) const -> void;

    auto set_position(glm::vec2 pos) -> void;
    auto translate_by(glm::vec2 translation) -> void;

private:
    GPUBuffer uniform_buffer_;

    glm::mat4 proj_mat_;
    glm::vec2 pos_;
};

} // namespace NoctisEngine::Rendering