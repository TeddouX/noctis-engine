#include <noctis_engine/rendering/mesh/mesh_manager.hpp>

#include <noctis_engine/rendering/mesh/default_meshes.hpp>


namespace NoctisEngine
{
    
MeshManager::MeshManager()
{
    auto buffer_flags = BufferFlag::MAP_WRITE_BIT 
        | BufferFlag::MAP_PERSISTENT_BIT 
        | BufferFlag::MAP_COHERENT_BIT;
    
    vbo_ = GPUBuffer{1, "mesh_manager_vbo", buffer_flags};
    ebo_ = GPUBuffer{1, "mesh_manager_ebo", buffer_flags};

    vertex_array_ = VertexArray{
        DEFAULT_VERTEX_ATTRIBUTES, 
        "mesh_manager_vertex_array", 
        false, false
    };

    vertex_array_.link_vbo(vbo_, sizeof(Vertex));
    vertex_array_.link_ebo(ebo_);
    RENDERING_LOGGER.debug("Created mesh manager buffers");
}

auto MeshManager::upload(const MeshData &mesh_data) -> MeshView
{
    MeshView mesh_view {
        .vertices_offset    = staged_vertices_.size(),
        .vertices_count     = mesh_data.vertices.size(),
        .indices_offset     = staged_indices_.size(),
        .indices_count      = mesh_data.indices.size(),
    };

    staged_vertices_.insert(staged_vertices_.end(), mesh_data.vertices.begin(), mesh_data.vertices.end());
    staged_indices_.insert(staged_indices_.end(), mesh_data.indices.begin(), mesh_data.indices.end());

    return mesh_view;
}

auto MeshManager::flush() -> void
{
    std::size_t vertices_size = staged_vertices_.size() * sizeof(Vertex);
    std::size_t indices_size = staged_indices_.size() * sizeof(std::uint32_t);

    bool vbo_resized = GPUBuffer::resize(vbo_, vertices_size, false);
    bool ebo_resized = GPUBuffer::resize(ebo_, indices_size, false);

    if (vbo_resized)
        vertex_array_.link_vbo(vbo_, sizeof(Vertex));

    if (ebo_resized)
        vertex_array_.link_ebo(ebo_);

    auto map_access = BufferMapAccess::MAP_WRITE_BIT 
        | BufferMapAccess::MAP_PERSISTENT_BIT 
        | BufferMapAccess::MAP_COHERENT_BIT;

    void *vbo_map = vbo_.map(map_access);
    void *ebo_map = ebo_.map(map_access);

    if (not vbo_map || not ebo_map)
        return;

    std::memcpy(vbo_map, staged_vertices_.data(), vertices_size);
    std::memcpy(ebo_map, staged_indices_.data(), indices_size);

    vbo_.unmap();
    ebo_.unmap();

    staged_vertices_.clear();
    staged_indices_.clear();
}

auto MeshManager::use(DrawList &draw_list) -> void
{
    vertex_array_.bind(draw_list);
}

auto MeshManager::delete_buffers() -> void
{
    vertex_array_.delete_gpu();
    ebo_.delete_gpu();
    vbo_.delete_gpu();

    RENDERING_LOGGER.debug("Deleted mesh manager");
}

auto MeshManager::ebo_handle() -> std::uint32_t
{
    return ebo_.gl_handle();
}

auto MeshManager::vao_handle() -> std::uint32_t
{
    return vertex_array_.vao_gl_handle();
}

} // namespace NoctisEngine