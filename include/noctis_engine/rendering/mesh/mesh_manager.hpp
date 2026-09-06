#pragma once 
#include "../gpu_buffer.hpp"
#include "../vertex_array.hpp"
#include "mesh_view.hpp"
#include "mesh_info.hpp"


namespace NoctisEngine
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
    /// @return The mesh view that represents the mesh on the GPU
    auto upload(const MeshData &mesh_data) -> MeshView;

    /// @brief Flushes all uploaded mesh data to the GPU
    auto flush() -> void;

    /// @brief Adds bind commands for this mesh manager's buffers
    /// @param draw_list The draw list that the commands should be added to
    auto use(DrawList &draw_list) -> void;

    /// @brief Deletes this mesh manager's buffers, it shouldn't be used afterwards
    auto delete_buffers() -> void;

    /// @brief Gets this mesh manager's EBO handle
    auto ebo_handle() -> std::uint32_t;
    
    /// @brief Gets this mesh manager's VAO handle
    auto vao_handle() -> std::uint32_t;

private:
    VertexArray vertex_array_;
    GPUBuffer   vbo_;
    GPUBuffer   ebo_;

    std::vector<Vertex>         staged_vertices_;
    std::vector<std::uint32_t>  staged_indices_;
};

} // namespace NoctisEngine
