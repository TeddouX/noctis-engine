#pragma once
#include <vector>
#include <cstring>

#include "draw_commands.hpp"
#include "gpu_buffer.hpp"
#include "../ecs/component/transform.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/ecs_world.hpp"
#include "../math/color.hpp"


namespace NoctisEngine::Rendering
{

class DrawList
{
public:
    DrawList() = default;
    ~DrawList() = default;

    auto bind_texture(std::uint32_t tex, std::uint32_t bind_point) -> void;

    auto bind_buffer(const GPUBuffer &buffer, BufferTarget target) -> void;
    auto bind_buffer_base(const GPUBuffer &buffer, BufferTarget target, std::uint32_t bind_point) -> void;

    auto bind_program(std::uint32_t prog) -> void;
    auto set_uniform(UniformInfo info) -> void;

    auto bind_vao(std::uint32_t vao) -> void;

    auto draw_mesh(MeshView mesh_view, const glm::mat4 &model_mat) -> void;
    auto draw_sprite_entity(ECS::Entity entity, const ECS::World &world) -> void;

    auto clear_screen(const Color &color, double depth = 1.0, bool clear_color = true, bool clear_depth = true) -> void;

    auto data() const -> const std::vector<std::byte> &;
    auto required_indirect_draw_cmds() const -> std::size_t;

private:
    std::vector<std::byte> data_;
    std::size_t req_indirect_draw_cmds{0};

    auto write_cmd_type(DrawCommandType type) -> void;

    template<typename T_>
    auto write(const T_ &value) -> void 
    {
        std::size_t offset = data_.size();
        data_.resize(offset + sizeof(T_));
        std::memcpy(data_.data() + offset, &value, sizeof(T_));
    }
};

} // namespace NoctisEngine::Rendering
