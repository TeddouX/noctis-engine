#include <noctis_engine/rendering/draw_list.hpp>

#include <noctis_engine/ecs/component/sprite.hpp>
#include <noctis_engine/rendering/mesh/default_meshes.hpp>


namespace NoctisEngine::Rendering
{

auto DrawList::bind_texture(std::uint32_t tex, std::uint32_t bind_point) -> void
{
    write_cmd_type(DrawCommandType::BIND_TEXTURE);
    write(BindTextureCmd{ tex, bind_point });
    
    set_uniform(UniformInfo{ 
        UniformType::INT, 
        "albedo", 
        static_cast<int>(bind_point) 
    });
}

auto DrawList::bind_buffer(const GPUBuffer &buffer, BufferTarget target) -> void
{
    write_cmd_type(DrawCommandType::BIND_BUFFER);
    write(BindBufferCmd{ buffer.gl_handle(), static_cast<std::uint32_t>(target) });
}

auto DrawList::bind_buffer_base(const GPUBuffer &buffer, BufferTarget target, std::uint32_t bind_point) -> void
{
    write_cmd_type(DrawCommandType::BIND_BUFFER_BASE);
    write(BindBufferBaseCmd{ buffer.gl_handle(), static_cast<std::uint32_t>(target), bind_point });
}

auto DrawList::bind_program(std::uint32_t prog) -> void
{
    write_cmd_type(DrawCommandType::BIND_PROGRAM);
    write(BindProgramCmd{ prog });
}

auto DrawList::bind_vao(std::uint32_t vao) -> void
{
    write_cmd_type(DrawCommandType::BIND_VAO);
    write(BindVAOCmd{ vao });
}

auto DrawList::set_uniform(UniformInfo info) -> void
{
    write_cmd_type(DrawCommandType::SET_UNIFORM);
    write(SetUniformCmd{ info });
}

auto DrawList::draw_mesh(MeshView mesh_view, const glm::mat4 &model_mat) -> void
{
    write_cmd_type(DrawCommandType::DRAW_MESH);
    write(DrawMeshCmd{ mesh_view, model_mat });

    req_indirect_draw_cmds++;
}

auto DrawList::draw_sprite_entity(ECS::Entity entity, const ECS::World &world) -> void
{
    auto *sprite = world.get_component<ECS::Sprite>(entity);
    if (not sprite) return;

    auto *transform = world.get_component<ECS::Transform>(entity);
    if (not transform) return;

    const Texture &tex = sprite->texture;
    const glm::mat4 &model_mat = transform->model_matrix();

    bind_texture(tex.gl_handle(), TextureSlots::ALBEDO);
    draw_mesh(
        DefaultMeshes::QUAD_MESH_VIEW, 
        glm::scale(model_mat, glm::vec3(tex.width(), tex.height(), 1))
    );
}

auto DrawList::clear_screen(const Color &color, double depth, bool clear_color, bool clear_depth) -> void
{
    write_cmd_type(DrawCommandType::CLEAR);
    write(ClearCmd{ color, depth, clear_color, clear_depth });
}

auto DrawList::write_cmd_type(DrawCommandType type) -> void
{
    write(type);
}

auto DrawList::data() const -> const std::vector<std::byte> &
{
    return data_;
}

auto DrawList::required_indirect_draw_cmds() const -> std::size_t
{
    return req_indirect_draw_cmds;
}

} // namespace NoctisEngine::Rendering
