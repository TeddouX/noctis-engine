#include <noctis_engine/rendering/mesh/mesh_manager.hpp>

#include <GL/gl.h>

#include <noctis_engine/rendering/vertex.hpp>
#include <noctis_engine/rendering/gpu_buffer_utils.hpp>
#include <noctis_engine/rendering/mesh/default_meshes.hpp>
#include <noctis_engine/core/logging.hpp>


namespace NoctisEngine::Rendering
{
    
MeshManager::MeshManager() 
    : vertices_off_(0zu)
    , indices_off_(0zu)
{
    vertices_gpu_buf_ = GPUBuffer(1, "mesh_manager_vertices");
    indices_gpu_buf_  = GPUBuffer(1, "mesh_manager_indices");

    create_vao();
    RENDERING_LOGGER.debug("Created mesh manager VAO");

    upload(DefaultMeshes::QUAD_MESH);
    RENDERING_LOGGER.debug("Uploaded default meshes");
}

auto MeshManager::upload(const MeshData &mesh) -> MeshView 
{
    size_t vertices_count = mesh.vertices.size(); 
    size_t vertices_size = vertices_count * sizeof(Vertex); 
    size_t indices_count = mesh.indices.size();
    size_t indices_size = indices_count * sizeof(std::uint32_t);

    vertices_cpu_buf_.insert(vertices_cpu_buf_.end(), mesh.vertices.begin(), mesh.vertices.end());
    indices_cpu_buf_.insert(indices_cpu_buf_.end(), mesh.indices.begin(), mesh.indices.end());

    bool verticesResized = resize_buffer(vertices_gpu_buf_, vertices_cpu_buf_);
    // TODO: reupload from offset
    vertices_gpu_buf_.write(get_cpu_buffer_view(vertices_cpu_buf_, 0, vertices_cpu_buf_.size()), 0);
    if(verticesResized)
        glVertexArrayVertexBuffer(VAO_, 0, vertices_gpu_buf_.gl_handle(), 0, sizeof(Vertex));

    bool indicesResized = resize_buffer(indices_gpu_buf_, indices_cpu_buf_);
    indices_gpu_buf_.write(get_cpu_buffer_view(indices_cpu_buf_, 0, indices_cpu_buf_.size()), 0);
    if (indicesResized)
        glVertexArrayElementBuffer(VAO_, indices_gpu_buf_.gl_handle());

    MeshView mv {
        .vertices_offset = vertices_off_ / sizeof(Vertex),
        .vertices_count = vertices_count,
        .indices_offset = indices_off_ / sizeof(std::uint32_t),
        .indices_count = indices_count,
    };

    vertices_off_ += vertices_size;
    indices_off_ += indices_size;

    return mv;
}

auto MeshManager::use(DrawList &draw_list) -> void 
{
    draw_list.bind_buffer(indices_gpu_buf_, BufferTarget::ELEMENT_ARRAY_BUFFER);
    draw_list.bind_vao(VAO_);
}

auto MeshManager::create_vao() -> void 
{
    constexpr std::string_view name = "mesh_manager_vao";

    // Create the VAO
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);
    glObjectLabel(GL_VERTEX_ARRAY, VAO_, name.size(), name.data());

    // Set its VBO and EBO
    glVertexArrayVertexBuffer(VAO_, 0, vertices_gpu_buf_.gl_handle(), 0, sizeof(Vertex));
    glVertexArrayElementBuffer(VAO_, indices_gpu_buf_.gl_handle());

    // Specify the vertice's format
    // Position
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexAttribBinding(0, 0);
    glEnableVertexAttribArray(0);
    
    // Normal
    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
    glVertexAttribBinding(1, 0);
    glEnableVertexAttribArray(1);

    // Tangent
    glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, tangent));
    glVertexAttribBinding(2, 0);
    glEnableVertexAttribArray(2);

    // Texture coordinate
    glVertexAttribFormat(3, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex_coords));
    glVertexAttribBinding(3, 0);
    glEnableVertexAttribArray(3);

    // Unbind it
    glBindVertexArray(0);
}


} // namespace NoctisEngine::Rendering