#pragma once 
#include "../gpu_buffer.hpp"
#include "../vertex_array.hpp"
#include "mesh_view.hpp"
#include "mesh_info.hpp"


namespace NoctisEngine::Rendering
{
    
/// @brief A class that helps you manage meshes. It stores them tightly in GPU memory only
class MeshManager
{
public:
    /// @brief Creates a mesh manager
    MeshManager();
    ~MeshManager() = default;

    /// @brief Uploads a mesh to the GPU
    /// @param mesh_data The mesh's data
    auto upload(const MeshData &mesh_data) -> void;

    /// @brief Adds bind commands for this mesh manager's buffers
    /// @param draw_list The draw list that the commands should be added to
    auto bind(DrawList &draw_list) -> void;

    /// @brief Deletes this mesh manager's buffers, it shouldn't be used afterwards
    auto delete_buffers() -> void;

private:
    VertexArray vertex_array_;
    GPUBuffer   vbo_;
    GPUBuffer   ebo_;
};

} // namespace NoctisEngine::Rendering
