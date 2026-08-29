#pragma once 
#include "../gpu_buffer.hpp"
#include "mesh_info.hpp"
#include "mesh_view.hpp"
#include "../draw_list.hpp"


namespace NoctisEngine::Rendering
{
    
class MeshManager
{
public:
    MeshManager();
    ~MeshManager() = default;

    auto upload(const MeshData &mesh) -> MeshView;
    auto use(DrawList &draw_list) -> void;

private:
    std::uint32_t VAO_;

    std::vector<Vertex>         vertices_cpu_buf_;
    std::vector<std::uint32_t>  indices_cpu_buf_;

    GPUBuffer                   vertices_gpu_buf_;
    GPUBuffer                   indices_gpu_buf_;

    std::size_t                 vertices_off_;
    std::size_t                 indices_off_;

    // std::vector<GPUBufferBlock> freeBlocks_;

    auto create_vao() -> void;
};

} // namespace NoctisEngine::Rendering
