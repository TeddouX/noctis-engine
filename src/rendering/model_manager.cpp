#include <noctis_engine/rendering/model_manager.hpp>

#include <noctis_engine/rendering/shader_bindings.hpp>
#include <noctis_engine/rendering/buffer_utils.hpp>
#include <noctis_engine/ecs/component/sprite.hpp>
#include <noctis_engine/ecs/component/transform_2d.hpp>
#include <noctis_engine/math/vec3.hpp>
#include <noctis_engine/math/matrix_math.hpp>


namespace NoctisEngine 
{

ModelManager::ModelManager()
{
    objects_ssbo_ = GPUBuffer{1, "model_manager_objects_ssbo", BufferFlag::DYNAMIC_STORAGE_BIT};
}

auto ModelManager::add_model_matrix(const Mat4x4 &matrix) -> void
{
    matrices_.push_back(matrix);
}

auto ModelManager::add_sprite_model(Entity entity, const ECSWorld &world) -> void
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
    Mat4x4 model_mat = transform->model_matrix();
    model_mat = scale_matrix(model_mat, Vec3(tex.width(), tex.height(), 1));


    add_model_matrix(model_mat);
}

auto ModelManager::use(DrawList &draw_list) -> void
{
    draw_list.bind_buffer_base(objects_ssbo_, BufferTarget::SHADER_STORAGE_BUFFER, ShaderBindings::OBJECTS_SSBO);
}

auto ModelManager::flush() -> void
{
    GPUBuffer::resize(objects_ssbo_, matrices_.size() * sizeof(Mat4x4));
    objects_ssbo_.write(get_cpu_read_view(matrices_, 0), 0);
    
    matrices_.clear();
}

}