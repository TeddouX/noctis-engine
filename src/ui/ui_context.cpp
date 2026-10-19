#include <noctis_engine/ui/ui_context.hpp>

#include <noctis_engine/rendering/mesh/default_meshes.hpp>
#include <noctis_engine/math/matrix_math.hpp>

#include "ui_render_data.hpp"


namespace NoctisEngine
{

UIContext::UIContext()
{
    ui_elems_ssbo_ = GPUBuffer{
        sizeof(ObjectData),
        "ui_ctx_elems_ssbo",
        BufferFlag::MAP_PERSISTENT_BIT | BufferFlag::MAP_COHERENT_BIT | BufferFlag::MAP_WRITE_BIT
    };

    vertex_array_ = VertexArray{DEBUG_VERTEX_ATTRIBUTES, sizeof(UIVertex), "ui_ctx_vertex_array"};
    vertex_array_.upload_vertices(UI_QUAD_VERTICES.data(),UI_QUAD_VERTICES.size());
    vertex_array_.upload_indices({ 0, 1, 2, 2, 3, 0 });
}

auto UIContext::enter_ctx() -> void
{
    // First context, bind every buffer
    if (curr_ctx_ == 0 && draw_lists_.empty())
    {
        if (draw_lists_.empty())
            draw_lists_.push_back(DrawList{});

        DrawList &root_draw_list = draw_lists_.front();
        ui_prog_.bind(root_draw_list);
        vertex_array_.bind(root_draw_list);
        root_draw_list.bind_buffer_base(ui_elems_ssbo_, BufferTarget::SHADER_STORAGE_BUFFER, 0);
    }

    curr_ctx_++;
    if (curr_ctx_ + 1 > draw_lists_.size())
        draw_lists_.push_back(DrawList{});
}

auto UIContext::has_ctx() -> bool
{
    return curr_ctx_ > 0;
}

auto UIContext::reset() -> void
{
    is_reset_ = true;
    draw_lists_[curr_ctx_ + 1] = DrawList{};
}

auto UIContext::is_reset() -> bool
{
    return is_reset_;
}

auto UIContext::exit_ctx() -> void
{
    curr_ctx_--;
}

auto UIContext::draw_textured_rect(const Vec2 &pos, const Rect &rect, const Texture &texture) -> void
{
    auto &draw_list = get_curr_draw_list();

    texture.bind(draw_list, TextureSlots::ALBEDO, TextureSlots::ALBEDO_NAME);
    
    Vec2 scale{rect.width, rect.height};
    float rotation = rect.rotation;    
    
    draw_list.draw_mesh(UI_QUAD_MESH_VIEW, model_matrix_2d(pos, scale, rotation));
}

auto UIContext::draw_colored_rect(const Vec2 &pos, const Rect &rect, const Color &color) -> void
{

}

auto UIContext::draw_text(const Vec2 &pos, const std::string_view &text) -> void
{

}

auto UIContext::get_curr_draw_list() -> DrawList &
{
    return draw_lists_[curr_ctx_ + 1];
}

auto UIContext::add_ui_elem(const Rect &rect) -> void
{

}

} // namespace NoctisEngine

