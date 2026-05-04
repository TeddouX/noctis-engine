#include <rendering/material/material_manager.hpp>

#include <rendering/buffer_utils.hpp>
#include <rendering/shader_bindings.hpp>

namespace NoctisEngine
{

MaterialManager::MaterialManager()
    : currKey_(0u)
{
    materialsSSBO_ = GPUBuffer(1, "material_manager_material_data");
    materialsSSBO_.bind_buffer_base(BufferTarget::SHADER_STORAGE_BUFFER, ShaderBindings::MATERIALS_BUFFER_SSBO);
}

auto MaterialManager::upload(const MaterialData &data) -> MaterialKey {
    materialsCPU_.push_back(data);

    if (resize_buffer(materialsSSBO_, materialsCPU_)) {
        materialsSSBO_.write(get_cpu_buffer_view(materialsCPU_, 0, materialsCPU_.size()), 0);
        materialsSSBO_.bind_buffer_base(BufferTarget::SHADER_STORAGE_BUFFER, ShaderBindings::MATERIALS_BUFFER_SSBO);
    }
    
    return MaterialKey{currKey_++};
}


auto MaterialManager::update_material(MaterialKey key, const MaterialData &newData) -> void {
    size_t offsetBytes = key.get() * sizeof(MaterialData);
    if (offsetBytes > materialsSSBO_.size_bytes())
        throw Exception("Key {} is invalid", key.get());

    materialsSSBO_.write(get_cpu_buffer_view(newData), offsetBytes);
}


auto MaterialManager::get_material(MaterialKey key) -> MaterialData {
    if (key.get() >= materialsCPU_.size())
        throw Exception("Key {} is invalid", key.get());
    return materialsCPU_[key.get()];
}

} // namespace NoctisEngine
