#pragma once
#include "gpu_buffer.hpp"
#include "material.hpp"
#include "../noctis_engine.hpp"


namespace NoctisEngine::Rendering
{
    
class MaterialManager 
{
public:
    MaterialManager();

    auto upload(const Material &data) -> MaterialKey;
    auto get_material(MaterialKey key) -> Material;
    auto update_material(MaterialKey key, const Material &newData) -> void;

private:
    std::vector<Material>   materialsCPU_;
    GPUBuffer               materialsSSBO_;
    int                     currKey_;
};

} // namespace NoctisEngine::Rendering