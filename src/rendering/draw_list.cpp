#include <noctis_engine/rendering/draw_list.hpp>

#include <noctis_engine/ecs/component/sprite.hpp>
#include <noctis_engine/ecs/component/transform_2d.hpp>
#include <noctis_engine/rendering/mesh/default_meshes.hpp>


namespace NoctisEngine
{

auto DrawList::bind_texture(std::uint32_t tex, std::uint32_t bind_point, std::string_view tex_name) -> void
{
    write_cmd_type(DrawCommandType::BIND_TEXTURE);
    write(BindTextureCmd{ tex, bind_point, tex_name });
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

auto DrawList::draw_mesh(const MeshView &mesh_view, const glm::mat4 &model_mat) -> void
{
    write_cmd_type(DrawCommandType::DRAW_MESH);
    write(DrawMeshCmd{ mesh_view, model_mat });

    req_indirect_draw_cmds++;
}

auto DrawList::draw_sprite_entity(Entity entity, const ECSWorld &world, const MeshView &quad_mv) -> void
{
    auto *sprite = world.get_component<Sprite2D>(entity);
    if (not sprite) 
    {
        RENDERING_LOGGER.warn("Sprite2D component is missing to draw a sprite entity");
        return;
    }

    auto *transform = world.get_component<Transform2D>(entity);
    if (not transform) 
    {
        RENDERING_LOGGER.warn("Transform2D component is missing to draw a sprite entity");
        return;
    }

    const Texture &tex = sprite->texture;
    glm::mat4 model_mat = transform->model_matrix();
    model_mat = glm::scale(model_mat, glm::vec3(tex.width(), tex.height(), 1));

    model_mat[3][2] = -sprite->draw_order;

    bind_texture(tex.gl_handle(), TextureSlots::ALBEDO, TextureSlots::ALBEDO_NAME);
    draw_mesh(quad_mv, model_mat);
}

auto DrawList::draw_triangles(std::size_t first, std::size_t count) -> void
{
    write_cmd_type(DrawCommandType::DRAW_TRIANGLES);
    write(DrawTrianglesCommand{ first, count });
}

auto DrawList::draw_lines(std::size_t first, std::size_t count) -> void
{
    write_cmd_type(DrawCommandType::DRAW_LINES);
    write(DrawLinesCommand{ first, count });
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

} // namespace NoctisEngine
