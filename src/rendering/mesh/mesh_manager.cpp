#include <noctis_engine/rendering/mesh/mesh_manager.hpp>

#include <noctis_engine/rendering/gpu_buffer_utils.hpp>
#include <noctis_engine/rendering/mesh/default_meshes.hpp>


namespace NoctisEngine::Rendering
{
    
MeshManager::MeshManager()
    : vertex_off_{0}
    , index_off_{0}
{
    vbo_ = GPUBuffer{1, "mesh_manager_vbo", BufferFlag::DYNAMIC_STORAGE_BIT};
    ebo_ = GPUBuffer{1, "mesh_manager_ebo", BufferFlag::DYNAMIC_STORAGE_BIT};

    vertex_array_ = VertexArray{
        DEFAULT_VERTEX_ATTRIBUTES, 
        "mesh_manager_vertex_array", 
        false, false
    };

    vertex_array_.link_vbo(vbo_, sizeof(Vertex));
    vertex_array_.link_ebo(ebo_);
    RENDERING_LOGGER.debug("Created mesh manager buffers");

    DefaultMeshes::QUAD_MESH_VIEW = upload(DefaultMeshes::QUAD_MESH);
    RENDERING_LOGGER.debug("Uploaded default meshes");
}

auto MeshManager::upload(const MeshData &mesh_data) -> MeshView
{
    std::size_t num_vertices = mesh_data.vertices.size();
    std::size_t vertices_size = num_vertices * sizeof(Vertex);
    std::size_t num_indices = mesh_data.indices.size();
    std::size_t indices_size = num_indices * sizeof(std::uint32_t);

    bool vbo_resized = resize_buffer(vbo_, vertices_size, true);
    bool ebo_resized = resize_buffer(ebo_, indices_size, true);

    vbo_.write(mesh_data.vertices.data(), vertices_size, vertex_off_);
    ebo_.write(mesh_data.indices.data(), indices_size, index_off_);

    if (vbo_resized)
        vertex_array_.link_vbo(vbo_, sizeof(Vertex));

    if (ebo_resized)
        vertex_array_.link_ebo(ebo_);

    MeshView mesh_view {
        .vertices_offset    = vertex_off_ / sizeof(Vertex),
        .vertices_count     = num_vertices,
        .indices_offset     = index_off_ / sizeof(std::uint32_t),
        .indices_count      = num_indices,
    };

    vertex_off_ += vertices_size;
    index_off_ += indices_size;

    return mesh_view;
}

auto MeshManager::use(DrawList &draw_list) -> void
{
    vertex_array_.bind(draw_list);
}

auto MeshManager::delete_buffers() -> void
{
    vertex_array_.delete_gpu();

    RENDERING_LOGGER.debug("Deleted mesh manager");
}

} // namespace NoctisEngine::Rendering