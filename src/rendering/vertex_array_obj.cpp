#include <noctis_engine/rendering/vertex_array_obj.hpp>

#include <GL/gl.h>

#include <noctis_engine/rendering/gpu_buffer_utils.hpp>


namespace NoctisEngine::Rendering
{
    
auto attrib_type_size_bytes(VertexAttribute::Type type) -> std::size_t;
    

VertexArrayObject::VertexArrayObject(
    const std::vector<VertexAttribute> &vertex_attribs, 
    std::string_view name, 
    bool create_buffers)
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glObjectLabel(GL_VERTEX_ARRAY, vao_, name.size(), name.data());

    if (create_buffers)
    {
        vbo_ = GPUBuffer(1, name);
        ebo_ = GPUBuffer(1, name);
    }

    std::uint32_t attrib_idx{0};
    std::uint32_t offset{0};

    for (const auto &vertex_attrib : vertex_attribs)
    {
        GLuint attrib_idx = vertex_attrib.index < 0 ? attrib_idx : vertex_attrib.index;

        glVertexAttribFormat(
            attrib_idx,
            vertex_attrib.num_components, 
            static_cast<GLenum>(vertex_attrib.type), 
            vertex_attrib.normalized ? GL_TRUE : GL_FALSE, 
            offset
        );
        glVertexAttribBinding(attrib_idx, 0);
        glEnableVertexAttribArray(attrib_idx);
    
        attrib_idx++;
        offset += attrib_type_size_bytes(vertex_attrib.type);
    }

    if (create_buffers)
    {
        // Offset should have the total size of one vertex by now
        link_vbo(vbo_, offset);
        link_ebo(ebo_);
    }
}

auto VertexArrayObject::upload_vertices(void *vertices, std::size_t size, std::size_t vertex_size) -> void
{
    bool resized = resize_buffer(vbo_, size);
    vbo_.write(vertices, size, 0);

    // Relink if resized because a new buffer was created
    if (resized)
        link_vbo(vbo_, vertex_size, 0);
}

auto VertexArrayObject::upload_indices(const std::vector<std::uint32_t> &indices) -> void
{
    bool resized = resize_buffer(ebo_, indices.size());
    ebo_.write(get_cpu_buffer_view(indices), 0);

    // Relink if resized because a new buffer was created
    if (resized)
        link_ebo(ebo_);
}

auto VertexArrayObject::link_vbo(const GPUBuffer &vbo_buf, std::size_t vertex_size, std::size_t first_el_off) -> void
{
    glVertexArrayVertexBuffer(vao_, 0, vbo_buf.gl_handle(), first_el_off, vertex_size);
}

auto VertexArrayObject::link_ebo(const GPUBuffer &ebo_buf) -> void
{
    glVertexArrayElementBuffer(vao_, ebo_buf.gl_handle());
}

auto VertexArrayObject::delete_gpu() -> void
{
    glDeleteVertexArrays(1, &vao_);
    vbo_.delete_gpu();
    ebo_.delete_gpu();
}

auto VertexArrayObject::vao_gl_handle() -> std::uint32_t
{
    return vao_;
}

auto VertexArrayObject::ebo_gl_handle() -> std::uint32_t
{
    return ebo_.gl_handle();
}

auto VertexArrayObject::vbo_gl_handle() -> std::uint32_t
{
    return vbo_.gl_handle();
}


auto attrib_type_size_bytes(VertexAttribute::Type type) -> std::size_t
{
    switch (type)
    {
        using enum VertexAttribute::Type;
        case BYTE: 
            return 1;

        case SHORT:
        case HALF_FLOAT:
            return 2;

        case INT:
        case FLOAT:
        case INT_2_10_10_10_REV:
        case UNSIGNED_INT_2_10_10_10_REV:
        case UNSIGNED_INT_10F_11F_11F_REV:
            return 4;

        case DOUBLE:
            return 8;
    }
}

} // namespace NoctisEngine::Rendering
