#include <noctis_engine/rendering/material_manager.hpp>

#include <noctis_engine/rendering/gpu_buffer_utils.hpp>
#include <noctis_engine/rendering/shader_bindings.hpp>


namespace NoctisEngine::Rendering
{

MaterialManager::MaterialManager()
    : currKey_(0u)
{
    materialsSSBO_ = GPUBuffer(1, "material_manager_material_data");
    materialsSSBO_.bind_buffer_base(BufferTarget::SHADER_STORAGE_BUFFER, ShaderBindings::MATERIALS_BUFFER_SSBO);
}

auto MaterialManager::upload(const Material &data) -> MaterialKey 
{
    materialsCPU_.push_back(data);

    bool resized = resize_buffer(materialsSSBO_, materialsCPU_);
    
    if (resized)
    {
        materialsSSBO_.write(get_cpu_buffer_view(materialsCPU_, 0, materialsCPU_.size()), 0);
        materialsSSBO_.bind_buffer_base(BufferTarget::SHADER_STORAGE_BUFFER, ShaderBindings::MATERIALS_BUFFER_SSBO);
    }
    else
    {
        std::size_t inserted_idx = materialsCPU_.size() - 1;
        materialsSSBO_.write(get_cpu_buffer_view(data), inserted_idx * sizeof(Material));
    }
    
    return MaterialKey{currKey_++};
}


auto MaterialManager::update_material(MaterialKey key, const Material &newData) -> void 
{
    std::size_t offsetBytes = key.get() * sizeof(Material);
    if (offsetBytes > materialsSSBO_.size_bytes())
        RENDERING_LOGGER.critical("Key {} is invalid", key.get());

    materialsSSBO_.write(get_cpu_buffer_view(newData), offsetBytes);
}


auto MaterialManager::get_material(MaterialKey key) -> Material 
{
    if (key.get() >= materialsCPU_.size())
        RENDERING_LOGGER.critical("Key {} is invalid", key.get());
        
    return materialsCPU_[key.get()];
}

} // namespace NoctisEngine::Rendering
